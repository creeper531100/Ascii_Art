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
    using ImageHandle::ImageHandle;

    SettingDataPack& console_init(int width, int height) {
        screen = new wchar_t[width * height];
        hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        return SettingDataPack::create().set_color(cv::COLOR_BGR2GRAY).set_dsize({width, height});
    }

    void video_interval(chrono::time_point<chrono::system_clock>* c_start) {
        std::this_thread::sleep_until(*c_start + chrono::microseconds(this->frame_interval));
        SetConsoleTitleW(to_wstring(frame_interval).c_str());
        *c_start = chrono::system_clock::now();
    }

    void ascii() {
        int width = super::param["console_show"]["ascii"]["width"];
        int height = super::param["console_show"]["ascii"]["height"];

        SettingDataPack pack = console_init(width, height);
        SetConsoleActiveScreenBuffer(hConsole);

        auto start = chrono::system_clock::now();
        super::basic_handle(pack, [&]() {
            for (int i = 0; i < pack.dsize.area(); i++) {
                screen[i] = super::lv[super::img.at<uchar>(i) / 4];
            }
            WriteConsoleOutputCharacterW(hConsole, screen, pack.dsize.area(), {0, 0}, &dwBytesWritten);
            video_interval(&start);
        });
    }

    void braille() {
        int width = super::param["console_show"]["braille"]["width"];
        int height = super::param["console_show"]["braille"]["height"];
        int thresh = super::param["console_show"]["thresh"];
        bool auto_thresh = false;

        map<string, wchar_t> map_pairs = init_words();
        SettingDataPack pack = console_init(width, height);
        vector<vector<char>> braille_string(width, vector<char>(height));

        if (thresh == -1) {
            auto_thresh = true;
        }
        SetConsoleActiveScreenBuffer(hConsole);

        auto start = chrono::system_clock::now();
        super::basic_handle(pack, [&]() {
            if (auto_thresh) {
                thresh = mean(super::img).val[0];
            }

            braille_create(braille_string, thresh, pack.dsize);
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
        });
    }
};
