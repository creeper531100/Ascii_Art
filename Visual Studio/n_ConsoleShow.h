﻿#pragma once

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

        float set_w = param["console_show"]["char_width"]; // 63 -> 設定尺寸
        float zoom = set_w / (float)original_size.width; // 63 / 1000 = 0.063 -> 計算縮放比例
        float set_h = (float)(original_size.height * zoom * 0.5); // 1376 * 0.063 = 86.688 (因為文字實際高度比寬度高，所以除以2平衡大小)

        if (mode == "braille") {
            if (type == IMG) {
                pack.set_dsize({(int)set_w * 2, (int)(set_h * 4)}); // 盲文寬度佔據2，長度4，依比例放大 
                pack.output_size = {(int)set_w, (int)set_h};
            }
            else
                pack.output_size = {pack.dsize.width / 2, pack.dsize.height / 4};
        }
        else if (mode == "ascii") {
            if (type == IMG) {
                pack.set_dsize({(int)set_w, (int)set_h});
                pack.output_size = pack.dsize;
            }
            else
                pack.output_size = pack.dsize;
        }

        if (type == VIDEO)
            hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

        screen = new wchar_t[pack.output_size.area()];
        return pack;
    }

    void video_interval(chrono::time_point<chrono::system_clock>* c_start) {
        std::this_thread::sleep_until(*c_start + chrono::microseconds(this->frame_interval));
        SetConsoleTitleW(to_wstring(frame_interval).c_str());
        *c_start = chrono::system_clock::now();
    }


    void written_file(FILE* file, SettingDataPack& pack, const wchar_t* buf) {
        if (type != IMG) {
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
        SettingDataPack pack = console_init("ascii");
        SetConsoleActiveScreenBuffer(hConsole);
        FILE* file = fopen(("out\\output" + get_timestamp() + ".txt").c_str(), "w+, ccs=UTF-16LE");

        auto start = chrono::system_clock::now();
        super::basic_handle(pack, [&]() {
            for (int i = 0; i < pack.output_size.area(); i++) {
                screen[i] = super::lv[super::img.at<uchar>(i) / 4];
            }
            if (type == VIDEO)
                WriteConsoleOutputCharacterW(hConsole, screen, pack.output_size.area(), {0, 0}, &dwBytesWritten);

            video_interval(&start);
            written_file(file, pack, screen);
            return nullptr;
        });
        fclose(file);

    }

    void braille() {
        SettingDataPack pack = console_init("braille");
        map<string, wchar_t> map_pairs = init_words();
        if (type == IMG)
            map_pairs = init_words(L'⠁');

        vector braille_string(pack.dsize.height, vector<char>(pack.dsize.width));
        bool auto_thresh = (pack.thresh == -1);
        int reverse = param["console_show"]["reverse"];
        SetConsoleActiveScreenBuffer(hConsole);
        FILE* file = fopen(("out\\output" + get_timestamp() + ".txt").c_str(), "w+, ccs=UTF-16LE");

        auto start = chrono::system_clock::now();
        super::basic_handle(pack, [&]() {
            if (auto_thresh) {
                pack.thresh = mean(super::img).val[0];
            }

            braille_create2(braille_string, pack.thresh, reverse);
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
            written_file(file, pack, screen);
            video_interval(&start);
            return nullptr;
        });
        fclose(file);
    }

    ~ConsoleShows() {
        delete screen;
    }
};
