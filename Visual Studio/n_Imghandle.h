#pragma once
#include "pch.h"

using namespace std;
using Json = nlohmann::json;

inline bool match_string(string keyword, vector<string> arr) {
    return std::find_if(arr.begin(), arr.end(), [&](string index) {
        return keyword.find(index) != std::string::npos;
        }) != arr.end();
}

struct SettingDataPack {
    cv::Size dsize;
    cv::ColorConversionCodes color;

    SettingDataPack() : dsize(cv::Size{ -1, -1 }), color((cv::ColorConversionCodes)-1) {
    }

    SettingDataPack& set_color(cv::ColorConversionCodes color) {
        this->color = color;
        return *this;
    }

    SettingDataPack& set_dsize(cv::Size dsize) {
        this->dsize = dsize;
        return *this;
    }

    static SettingDataPack create() {
        return SettingDataPack();
    }
};

class ImageHandle {
protected:
    string file_path;
    Json param;
    cv::Mat img;
    enum FileType { NONE, IMG, VIDEO } type;
    cv::VideoCapture cap;
    cv::VideoWriter writer;

    double frame_FPS;
    double frame_total;
    int frame_interval;
    int encoding;

    cv::Size original_video_size;
    string lv = " .'`^,:;l!i><~+_--?][}{)(|/rxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
public:
    ImageHandle(string path, Json param) : file_path(path), param(param) {
        if (match_string(path, { ".jpg", ".JPG", ".png", ".PNG", ".tiff" }) == true) {
            //�P�_�Ϥ�
            this->img = cv::imread(path);
            cout << "Resize Size: " << this->img.size().height << "x" << this->img.size().width << endl;
            this->type = IMG;
        }
        else if (match_string(path, { ".mp4", ".mp3", ".gif" })) {
            this->cap = cv::VideoCapture(path);
            this->frame_FPS = this->cap.get(cv::CAP_PROP_FPS);
            this->frame_total = this->cap.get(cv::CAP_PROP_FRAME_COUNT);
            this->frame_interval = (int)((1.0 / this->frame_FPS) * 1000000.0);
            this->original_video_size = cv::Size(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT));
            this->type = VIDEO;
            this->encoding = cv::VideoWriter::fourcc('D', 'I', 'V', 'X');
        }
    }

    void img_handle(SettingDataPack pack) {
        cv::resize(this->img, this->img, pack.dsize, 0, 0, cv::INTER_CUBIC);
        cv::cvtColor(this->img, this->img, pack.color);
    }

    void print_output_info(time_t t_start) {
        system(("ffmpeg -i tempvideo.mp4 -i " + this->file_path + " -c copy -map 0:v:0 -map 1:a:0 output_video.mp4").c_str());
        int totalTime = difftime(time(NULL), t_start);
        printf("\nused %02d:%02d\n", totalTime / 60, totalTime % 60);
        remove("tempvideo.mp4");
        system("pause");
    }

    void create_written(cv::Size origin_size, cv::Size set_size) {
        if (set_size.height == -1)
            set_size = origin_size;

        cout << "Resize Size: " << origin_size.width << "x" << origin_size.height << endl;
        cout << "Output Size: " << set_size.width << "x" << set_size.height << endl;

        writer = cv::VideoWriter("tempvideo.mp4", this->encoding, this->frame_FPS, set_size);
    }

    ImageHandle& basic_handle(SettingDataPack pack, function<void()>&& func) {
        time_t t_start = time(NULL);
        if (type == IMG) {
            this->img_handle(pack);
            func();
        }
        else if (type == VIDEO) {
            while (1) {
                this->cap >> this->img;
                if (this->img.empty())
                    break;
                this->img_handle(pack);
                func();
            }
        }
        print_output_info(t_start);
        return *this;
    }

    void braille_create(vector<vector<char>>& deep_arr, int brightness_threshold, cv::Size& dsize) {
        for (int i = 0; i < dsize.height; i++) {
            for (int j = 1, k = 0; j < dsize.width; j += 2, k++) {
                if (this->img.at<uchar>(i, j - 1) > brightness_threshold) {
                    if (this->img.at<uchar>(i, j) > brightness_threshold)
                        deep_arr[i][k] = 'm';
                    else
                        deep_arr[i][k] = 'y';
                }
                else {
                    if (this->img.at<uchar>(i, j) > brightness_threshold)
                        deep_arr[i][k] = 'z';
                    else
                        deep_arr[i][k] = 'k';
                }
            }
        }
    }
};
