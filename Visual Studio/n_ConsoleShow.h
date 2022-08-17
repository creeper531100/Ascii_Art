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
                               .thresh_detect();

        screen = new wchar_t[pack.dsize.area()];
        hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        return pack;
    }

    void video_interval(chrono::time_point<chrono::system_clock>* c_start) {
        std::this_thread::sleep_until(*c_start + chrono::microseconds(this->frame_interval));
        SetConsoleTitleW(to_wstring(frame_interval).c_str());
        *c_start = chrono::system_clock::now();
    }

    void ascii() {
        SettingDataPack pack = console_init("ascii");
        SetConsoleActiveScreenBuffer(hConsole);

        auto start = chrono::system_clock::now();
        super::basic_handle(pack, [&]() {
            for (int i = 0; i < pack.dsize.area(); i++) {
                screen[i] = super::lv[super::img.at<uchar>(i) / 4];
            }
            WriteConsoleOutputCharacterW(hConsole, screen, pack.dsize.area(), {0, 0}, &dwBytesWritten);
            video_interval(&start);
            return nullptr;
        });
    }

    void braille() {
        SettingDataPack pack = console_init("braille");
        map<string, wchar_t> map_pairs = init_words();
        vector<vector<char>> braille_string(pack.dsize.height, vector<char>(pack.dsize.width));
        bool auto_thresh = (pack.thresh == -1);

        SetConsoleActiveScreenBuffer(hConsole);

        auto start = chrono::system_clock::now();
        super::basic_handle(pack, [&]() {
            if (auto_thresh) {
                pack.thresh = mean(super::img).val[0];
            }

            braille_create2(braille_string, pack.thresh);
            for (int i = 3, pixel = 0; i < braille_string.size(); i += 4) {
                for (int j = 0; j < pack.dsize.height; j++, pixel++) {
                    string buf = {
                            braille_string[i - 3][j], braille_string[i - 2][j], braille_string[i - 1][j],
                            braille_string[i][j]
                        };
                    this->screen[pixel] = map_pairs[buf];
                }
            }
            WriteConsoleOutputCharacterW(hConsole, screen, pack.dsize.area(), {0, 0}, &dwBytesWritten);
            video_interval(&start);
            return nullptr;
        });
    }

    ~ConsoleShows() {
        delete screen;
    }
};
