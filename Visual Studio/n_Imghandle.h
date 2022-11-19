#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "header.h"
#include "pch.h"
#include "SettingPack.h"

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

class ImageHandle {
protected:
    string file_path;
    Json param;
    cv::Mat img;
    cv::Mat orig_img;
    cv::VideoCapture cap;
    cv::VideoWriter writer;

    enum class FileType { NONE, IMG, VIDEO, ONE_BY_ONE } type;

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
            this->type = FileType::IMG;
            this->original_size = img.size();
            cout << "Resize Size: " << this->orig_img.size().height << "x" << this->orig_img.size().width << endl;
        }
        else if (match_string(path, {".mp4", ".mp3", ".gif"})) {
            this->cap = cv::VideoCapture(path);
            this->frame_FPS = this->cap.get(cv::CAP_PROP_FPS);
            this->frame_total = this->cap.get(cv::CAP_PROP_FRAME_COUNT);
            this->frame_interval = (int)((1.0 / this->frame_FPS) * 1000000.0);
            this->original_size = cv::Size(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT));
            this->type = FileType::VIDEO;
            this->encoding = cv::VideoWriter::fourcc('D', 'I', 'V', 'X');
        }
    }

    void print_output_info(time_t t_start) {
        #ifndef _DEBUG
        if (type == FileType::VIDEO) {
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
        time_t t_start = time(NULL);

        if (pack.thresh == -2) {
            pack.thresh = 0;
            this->type = FileType::ONE_BY_ONE;
            this->frame_FPS = 30.0;
            this->encoding = cv::VideoWriter::fourcc('D', 'I', 'V', 'X');
        }

        if (type == FileType::VIDEO || type == FileType::ONE_BY_ONE) {
            switch (pack.output_size.width) {
            case OutputSizeMode::DISABLE:
                break;
            case OutputSizeMode::ORIGIN_SIZE:
                create_written(original_size);
                break;
            default:
                create_written(pack.output_size);
                break;
            }
        }

        while (1) {
            if (type == FileType::VIDEO)
                this->cap >> this->orig_img;

            this->orig_img.copyTo(this->img);
            if (this->orig_img.empty())
                break;

            if (pack.output_size.width != OutputSizeMode::ORIGIN_SIZE)
                resize(this->img, this->img, pack.dsize, 0, 0, cv::INTER_CUBIC);

            if (pack.color != AUTO_DETECT)
                cv::cvtColor(this->img, this->img, pack.color);

            cv::Mat* output_mat = func();

            if (!output_mat) {
                if (type == FileType::VIDEO)
                    continue;
                break;
            }

            if (process % 30 == 0) {
                cv::imshow("preview", *output_mat);
                cv::waitKey(1);
            }

            if (type == FileType::IMG || type == FileType::ONE_BY_ONE) {
                imwrite("out\\output_pic" + get_timestamp() + "-" + std::to_string(pack.thresh) + ".png", *output_mat);
                if (type == FileType::ONE_BY_ONE && pack.thresh <= 255) {
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
    }

    virtual void ascii() {
        fmt::print(u8"沒有這個功能\n");
    }

    virtual void braille() {
        fmt::print(u8"沒有這個功能\n");
    }

    virtual void ascii(int) {
        fmt::print(u8"沒有這個功能\n");
    }

    virtual void qt() {
        fmt::print(u8"沒有這個功能\n");
    }

    ImageHandle& basic_handle2(ConsoleShowPack& pack, function<cv::Mat* ()>&& func) {
        int process = 0;
        time_t t_start = time(NULL);

        if (pack.get_mode() == OutputSetting::ONE_BY_ONE) {
            pack.thresh = 0;
            this->frame_FPS = 30.0;
            this->type = FileType::ONE_BY_ONE;
            this->encoding = cv::VideoWriter::fourcc('D', 'I', 'V', 'X');
        }

        if (type == FileType::VIDEO || type == FileType::ONE_BY_ONE) {
            switch (pack.output_mode) {
            case OutputSizeMode2::DISABLE:
                break;
            case OutputSizeMode2::ORIGIN_SIZE:
                create_written(original_size);
                break;
            default:
                create_written(pack.output_size);
                break;
            }
        }

        while (1) {
            if (type == FileType::VIDEO)
                this->cap >> this->orig_img;

            this->orig_img.copyTo(this->img);
            if (this->orig_img.empty())
                break;

            if (pack.output_mode != OutputSizeMode2::ORIGIN_SIZE)
                resize(this->img, this->img, pack.dsize, 0, 0, cv::INTER_CUBIC);

            if (pack.color != (int)OutputSetting::AUTO)
                cv::cvtColor(this->img, this->img, pack.color);

            cv::Mat* output_mat = func();

            if (!output_mat) {
                if (type == FileType::VIDEO)
                    continue;
                break;
            }

            if (process % 30 == 0) {
                cv::imshow("preview", *output_mat);
                cv::waitKey(1);
            }

            if (type == FileType::IMG || type == FileType::ONE_BY_ONE) {
                imwrite("out\\output_pic" + get_timestamp() + "-" + std::to_string(pack.thresh) + ".png", *output_mat);
                if (type == FileType::ONE_BY_ONE && pack.thresh <= 255) {
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
};
