﻿#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "pch.h"
#define AUTO_DETECT -1
#define AUTO_RESIZE -1, -1


using namespace std;
using Json = nlohmann::json;

enum OutputMode { DISABLE = -1, ORIGIN_SIZE = -2 };

inline bool match_string(string keyword, vector<string> arr) {
    return std::find_if(arr.begin(), arr.end(), [&](string index) {
        return keyword.find(index) != std::string::npos;
    }) != arr.end();
}

inline string get_timestamp() {
    time_t now = time(0);
    tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%d_%M_%S", &tstruct);
    return string(buf);
}

struct SettingDataPack {
    cv::Size dsize;
    cv::Size output_size;
    int thresh;
    cv::ColorConversionCodes color;
    Json param;
    string func_name;


    SettingDataPack(Json param, string func_name) :
        dsize(cv::Size{AUTO_RESIZE}),
        color((cv::ColorConversionCodes)AUTO_DETECT),
        param(param),
        output_size(cv::Size{AUTO_RESIZE}),
        func_name(func_name) {
    }

    SettingDataPack& set_color(cv::ColorConversionCodes color) {
        this->color = color;
        return *this;
    }

    SettingDataPack& set_dsize(cv::Size dsize) {
        this->dsize = dsize;
        return *this;
    }

    SettingDataPack& set_output_mode(OutputMode mode) {
        this->output_size.width = (int)mode;
        return *this;
    }

    SettingDataPack& enable_thresh_detect() {
        this->thresh = param[func_name]["thresh"];
        return *this;
    }

    /**
     * \brief 設定縮放尺寸
     * \param mode:                  模式名稱
     * \param original_video_size:   原始尺寸
     * \param thumbnail_size:        縮圖尺寸
     * \param zoom:                  文字寬度比例
     * \return SettingDataPack&
     */
    SettingDataPack& set_dsize(const char* mode, cv::Size& original_video_size, cv::Size thumbnail_size = {8, 16},
                               pair<int, int> zoom = {1, 1}) {
        /*
         * width除8 => 因為img被resize了，輸出圖像必須被擴充至原始解析度(thumbnail縮圖，乘上縮圖即原始尺寸)
         * width除zoom => 一個文字占據兩格寬度
         * width除8再乘8 => 這邊是為了找近似解析度，先除8去掉小數，在乘8回到近似的原始解析度
         * 同理height
         */
        this->dsize = {param[func_name][mode]["width"], param[func_name][mode]["height"]};
        bool auto_reszie = this->dsize.width == AUTO_DETECT || this->dsize.height == AUTO_DETECT;

        if (auto_reszie) {
            this->dsize.width = original_video_size.width / (thumbnail_size.width / zoom.first);
            this->dsize.height = original_video_size.height / (thumbnail_size.height / zoom.second);
        }

        if (zoom.first != 1 && auto_reszie) {
            this->dsize.width = (int)(this->dsize.width / thumbnail_size.width) * thumbnail_size.width;
            this->dsize.height = (int)(this->dsize.height / thumbnail_size.height) * thumbnail_size.height;
        }
        return *this;
    }

    static SettingDataPack create(Json param, string func_name = "") {
        return SettingDataPack(param, func_name);
    }
};

class ImageHandle {
protected:
    string file_path;
    Json param;
    cv::Mat img;
    cv::Mat orig_img;
    cv::VideoCapture cap;
    cv::VideoWriter writer;

    enum FileType { NONE, IMG, VIDEO } type;

    double frame_FPS;
    double frame_total;
    int frame_interval;
    int encoding;

    cv::Size original_size;
    string lv = " .'`^,:;l!i><~+_--?][}{)(|/rxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
public:
    ImageHandle(string path, Json param) : file_path(path), param(param) {
        if (match_string(path, {".jpg", ".JPG", ".png", ".PNG", ".tiff"}) == true) {
            //判斷圖片
            this->orig_img = cv::imread(path);
            this->orig_img.copyTo(this->img);
            cout << "Resize Size: " << this->orig_img.size().height << "x" << this->orig_img.size().width << endl;
            this->type = IMG;
            this->original_size = img.size();
        }
        else if (match_string(path, {".mp4", ".mp3", ".gif"})) {
            this->cap = cv::VideoCapture(path);
            this->frame_FPS = this->cap.get(cv::CAP_PROP_FPS);
            this->frame_total = this->cap.get(cv::CAP_PROP_FRAME_COUNT);
            this->frame_interval = (int)((1.0 / this->frame_FPS) * 1000000.0);
            this->original_size = cv::Size(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT));
            this->type = VIDEO;
            this->encoding = cv::VideoWriter::fourcc('D', 'I', 'V', 'X');
        }
    }

    void print_output_info(time_t t_start) {
#ifndef _DEBUG
        if (type == VIDEO) {
            writer.release();
            Sleep(1000);
            system(
                ("ffmpeg -i out\\tempvideo.mp4 -i " + this->file_path +
                    " -c copy -map 0:v:0 -map 1:a:0 out\\output_video" + get_timestamp() + ".mp4").
                c_str());
            int totalTime = difftime(time(NULL), t_start);
            printf("\nused %02d:%02d\n", totalTime / 60, totalTime % 60);
            remove("out\\tempvideo.mp4");
            system("pause");
        }
#endif
    }

    void create_written(cv::Size set_size) {
        cout << "Output Size: " << set_size.width << "x" << set_size.height << endl;
        writer = cv::VideoWriter("out\\tempvideo.mp4", this->encoding, this->frame_FPS, set_size);
    }

    ImageHandle& basic_handle(SettingDataPack& pack, function<cv::Mat*()>&& func) {
        int process = 0;
        bool enable_array_thresh = (pack.thresh == -2);
        time_t t_start = time(NULL);

        if (enable_array_thresh) {
            pack.thresh = 0;
            this->frame_FPS = 30.0;
            this->encoding = cv::VideoWriter::fourcc('D', 'I', 'V', 'X');
        }

        if (type == VIDEO || enable_array_thresh) {
            switch (pack.output_size.width) {
            case OutputMode::DISABLE:
                break;
            case OutputMode::ORIGIN_SIZE:
                create_written(original_size);
                break;
            default:
                create_written(pack.output_size);
                break;
            }
        }

        while (1) {
            if (type == VIDEO)
                this->cap >> this->orig_img;

            this->orig_img.copyTo(this->img);
            if (this->orig_img.empty())
                break;

            if (pack.output_size.width != OutputMode::ORIGIN_SIZE)
                resize(this->img, this->img, pack.dsize, 0, 0, cv::INTER_CUBIC);

            if (pack.color != AUTO_DETECT)
                cv::cvtColor(this->img, this->img, pack.color);

            cv::Mat* output_mat = func();

            if (!output_mat) {
                if (type == VIDEO)
                    continue;
                break;
            }

            if (process % 30 == 0) {
                cv::imshow("preview", *output_mat);
                cv::waitKey(1);
            }

            if (type == IMG) {
                imwrite("out\\output_pic" + get_timestamp() + "-" + std::to_string(pack.thresh) + ".png", *output_mat);
                if (enable_array_thresh && pack.thresh <= 255) {
                    fmt::print(u8"進度: {}%\r", (pack.thresh++ / 256.0) * 100.0);
                    writer.write(*output_mat);
                    continue;
                }
                break;
            }
            fmt::print(u8"進度: {}%\r", (process++ / frame_total) * 100);
            writer.write(*output_mat);
        }

        print_output_info(t_start);
        return *this;
    }

    void braille_create2(vector<vector<char>>& deep_arr, int threshold, bool rev = 0) {
        for (int i = 0; i < deep_arr.size(); i++) {
            for (int j = 1; j < deep_arr[0].size(); j += 2) {
                if (thresh_cmp(rev, threshold, this->img.at<uchar>(i, j - 1))) {
                    if (thresh_cmp(rev, threshold, this->img.at<uchar>(i, j)))
                        deep_arr[i][j / 2] = 'm';
                    else
                        deep_arr[i][j / 2] = 'y';
                }
                else {
                    if (thresh_cmp(rev, threshold, this->img.at<uchar>(i, j)))
                        deep_arr[i][j / 2] = 'z';
                    else
                        deep_arr[i][j / 2] = 'k';
                }
            }
        }
    }

    bool thresh_cmp(int reverse, int val1, int val2) {
        if (reverse) {
            return val1 > val2;
        }
        return val1 < val2;
    };
};
