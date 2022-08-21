#pragma once

#include "n_Imghandle.h"
#include "n_initword.h"
#include "pch.h"

class ConsoleShows : public ImageHandle {
private:
    using super = ImageHandle;
    wchar_t* screen;
    HANDLE hConsole;
    DWORD dwBytesWritten;
public:
    using super::super;

    SettingDataPack console_init(const char* mode) {
        SettingDataPack pack = SettingDataPack::create(param, "console_show")
                               .set_color(cv::COLOR_BGR2GRAY)
                               .set_dsize(mode, this->original_size)
                               .enable_thresh_detect()
                               .set_output_mode(OutputMode::DISABLE);

        if (mode == "braille") {
            pack.output_size = {pack.dsize.width / 2, pack.dsize.height / 4};
            if (type == IMG) {
                float set_w = param["console_show"]["char_width"];
                float zoom = set_w / (float)original_size.width;
                float set_h = (float)(original_size.height * zoom * 0.5);

                pack.set_dsize({(int)set_w * 2, (int)set_h * 4});
                pack.output_size = {(int)set_w, (int)set_h};
            }
        }
        else if (mode == "ascii"){
            pack.output_size = pack.dsize;
            if (type == IMG) {
                float set_w = param["console_show"]["char_width"];
                float zoom = set_w / (float)original_size.width;
                float set_h = original_size.height * zoom;

                pack.set_dsize({ (int)set_w * 2, (int)set_h });
                pack.output_size = pack.dsize;
            }
        }

        if(type == VIDEO)
            hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

        screen = new wchar_t[pack.output_size.area()];
        return pack;
    }

    void video_interval(chrono::time_point<chrono::system_clock>* c_start) {
        std::this_thread::sleep_until(*c_start + chrono::microseconds(this->frame_interval));
        SetConsoleTitleW(to_wstring(frame_interval).c_str());
        *c_start = chrono::system_clock::now();
    }


    void written_file(FILE* fLog, const wchar_t* buf, SettingDataPack& pack, cv::Size zoom = {1, 1}) {
        if (type != IMG) {
            return;
        }

        if (!fLog) {
            return;
        }

        for (int i = 0; i < pack.output_size.height; i++) {
            for (int j = 0; j < pack.output_size.width; j++) {
                const wchar_t buffer[] = {buf[i * pack.output_size.width + j], L'\0'};
                fwrite(buffer, sizeof(wchar_t), wcslen(buffer), fLog);
            }
            fwrite(L"\n", sizeof(wchar_t), wcslen(L"\n"), fLog);
        }
    }

    void ascii() {
        SettingDataPack pack = console_init("ascii");
        SetConsoleActiveScreenBuffer(hConsole);
        FILE* fLog = _wfopen(L"out\\output.txt", L"w+, ccs=UTF-16LE");

        auto start = chrono::system_clock::now();
        super::basic_handle(pack, [&]() {
            for (int i = 0; i < pack.output_size.area(); i++) {
                screen[i] = super::lv[super::img.at<uchar>(i) / 4];
            }
            if (type == VIDEO)
                WriteConsoleOutputCharacterW(hConsole, screen, pack.output_size.area(), {0, 0}, &dwBytesWritten);

            video_interval(&start);
            written_file(fLog, screen, pack);
            return nullptr;
        });
        fclose(fLog);

    }

    void braille() {
        SettingDataPack pack = console_init("braille");

        map<string, wchar_t> map_pairs = init_words();
        vector braille_string(pack.dsize.height, vector<char>(pack.dsize.width));
        bool auto_thresh = (pack.thresh == -1);
        SetConsoleActiveScreenBuffer(hConsole);
        FILE* fLog = _wfopen(L"out\\output.txt", L"w+, ccs=UTF-16LE");

        auto start = chrono::system_clock::now();
        super::basic_handle(pack, [&]() {
            if (auto_thresh) {
                pack.thresh = mean(super::img).val[0];
            }

            braille_create2(braille_string, pack.thresh, 1);
            for (int row = 3, i = 0; i < pack.output_size.height; row += 4, i++) {
                for (int col = 0; col < pack.output_size.width; col++) {
                    string buf = {
                            braille_string[row - 3][col], braille_string[row - 2][col], braille_string[row - 1][col],
                            braille_string[row][col]
                        };
                    this->screen[i * pack.output_size.width + col] = map_pairs[buf];
                }
            }
            if (type == VIDEO)
                WriteConsoleOutputCharacterW(hConsole, screen, pack.output_size.area(), {0, 0}, &dwBytesWritten);
            written_file(fLog, screen, pack);
            video_interval(&start);
            return nullptr;
        });
        fclose(fLog);
    }

    ~ConsoleShows() {
        delete screen;
    }
};
