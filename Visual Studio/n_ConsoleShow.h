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

    ConsoleShowPack console_init2(const char* mode) {
        ConsoleShowPack pack = ConsoleShowPack::create(param)
                               .set_color(cv::COLOR_BGR2GRAY)
                               .set_output_mode(OutputSizeMode2::DISABLE)
                               .set_dsize(mode);

        float set_w = param["console_show"]["char_width"]; // 63 -> 設定尺寸
        float zoom = set_w / (float)original_size.width; // 63 / 1000 = 0.063 -> 計算縮放比例
        float set_h = (float)(original_size.height * zoom * 0.5); // 1376 * 0.063 = 86.688 (因為文字實際高度比寬度高，所以除以2平衡大小)
        pack.output_size = pack.dsize;

        if (mode == "braille") {
            pack.output_size = { pack.dsize.width / 2, pack.dsize.height / 4 };
        }

        if (type == FileType::IMG) {
            if (mode == "braille") {
                pack.dsize = { (int)set_w * 2, (int)(set_h * 4) }; // 盲文寬度佔據2，長度4，依比例放大 
                pack.output_size = { (int)set_w, (int)set_h };
            }
            else {
                pack.dsize = { (int)set_w, (int)set_h };
                pack.output_size = pack.dsize;
            }
        }

        screen = new wchar_t[pack.output_size.area()];
        if (type == FileType::VIDEO) {
            hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        }
        return pack;
    }

    void video_interval(chrono::time_point<chrono::system_clock>* c_start) {
        std::this_thread::sleep_until(*c_start + chrono::microseconds(this->frame_interval));
        SetConsoleTitleW(to_wstring(frame_interval).c_str());
        *c_start = chrono::system_clock::now();
    }

    void written_file2(FILE* file, ConsoleShowPack& pack, const wchar_t* buf) {
        if (type != FileType::IMG) {
            return;
        }
        if (!file) {
            return;
        }
        for (int i = 0; i < pack.output_size.height; i++) {
            for (int j = 0; j < pack.output_size.width; j++) {
                const wchar_t buffer[] = {buf[i * pack.output_size.width + j], L'\0'};
                fwrite(buffer, sizeof(wchar_t), wcslen(buffer), file);
            }
            fwrite(L"\n", sizeof(wchar_t), wcslen(L"\n"), file);
        }
    }

    void ascii() {
        ConsoleShowPack pack = console_init2("ascii");
        SetConsoleActiveScreenBuffer(hConsole);
        FILE* file = fopen(("out\\output" + get_timestamp() + ".txt").c_str(), "w+, ccs=UTF-16LE");
        auto start = chrono::system_clock::now();
        super::basic_handle2(pack, [&]() {
            for (int i = 0; i < pack.output_size.area(); i++) {
                screen[i] = super::lv[super::img.at<uchar>(i) / 4];
            }

            if (type == FileType::IMG) {
                written_file2(file, pack, screen);
                return nullptr;
            }

            WriteConsoleOutputCharacterW(hConsole, screen, pack.output_size.area(), { 0, 0 }, &dwBytesWritten);
            video_interval(&start);
            return nullptr;
        });
        fclose(file);
    }

    void braille() {
        ConsoleShowPack pack = console_init2("braille");
        wchar_t first_chr = (type == FileType::IMG) ? L'⠁' : L' ';

        map<string, wchar_t> map_pairs = init_words(first_chr);

        vector braille_string(pack.dsize.height, vector<char>(pack.dsize.width));
        bool auto_thresh = (pack.get_mode() == OutputSetting::AUTO);
        SetConsoleActiveScreenBuffer(hConsole);

        FILE* file = fopen(("out\\output" + get_timestamp() + ".txt").c_str(), "w+, ccs=UTF-16LE");
        auto start = chrono::system_clock::now();
        super::basic_handle2(pack, [&]() {
            if (auto_thresh) {
                pack.thresh = mean(super::img).val[0];
            }

            braille_create2(braille_string, pack.thresh, pack.get_reverse());
            for (int row = 3, i = 0; i < pack.output_size.height; row += 4, i++) {
                for (int col = 0; col < pack.output_size.width; col++) {
                    string buf = {
                        braille_string[row - 3][col], braille_string[row - 2][col], braille_string[row - 1][col],
                        braille_string[row][col]
                    };
                    this->screen[i * pack.output_size.width + col] = map_pairs[buf];
                }
            }

            if (type == FileType::IMG) {
                written_file2(file, pack, screen);
                return nullptr;
            }

            WriteConsoleOutputCharacterW(hConsole, screen, pack.output_size.area(), { 0, 0 }, &dwBytesWritten);
            video_interval(&start);
            return nullptr;
        });
        fclose(file);
    }

    ~ConsoleShows() {
        delete screen;
    }
};
