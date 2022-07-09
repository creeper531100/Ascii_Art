#pragma once
#include "n_Imghandle.h"
#include "pch.h"

inline std::vector<std::string> split(std::string split_str, std::string&& delimiter) {
    //分割特定字元
    int pos_start = 0, pos_end, delim_len = delimiter.length();
    std::vector<std::string> res;

    while ((pos_end = split_str.find(delimiter, pos_start)) != std::string::npos) {
        std::string token = split_str.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(split_str.substr(pos_start));
    return res;
}

inline unordered_map<string, cv::Mat> read_img(string path, vector<cv::Mat>&& mats = {}) {
    unordered_map<string, cv::Mat> vec;
    vector<string> names;
    cv::glob(path, names);
    for (auto& row : names) {
        cv::Mat mat = cv::imread(row);
        string name = split(row, "\\")[2];
        vec[name] = mat;
        mats.push_back(mat);
    }
    return vec;
}

class CollageOutput : public ImageHandle {
public:
    using ImageHandle::ImageHandle;
    using super = ImageHandle;

    void ascii() {
        SettingDataPack pack = SettingDataPack::create(param, "collage_output")
                               .set_color(cv::COLOR_BGR2GRAY)
                               .set_dsize("ascii", original_video_size);
        int process = 0;
        cv::Size output_size = {pack.dsize.width * 8, pack.dsize.height * 16};
        cv::Mat output_mat(output_size.height, output_size.width, CV_8UC3);

        if (super::type == IMG)
            pack.dsize = cv::Size(super::img.cols / 8, super::img.rows / 16);
        else
            super::create_written(pack.dsize, output_size);

        vector<cv::Mat> mats;
        read_img("font\\font\\", std::move(mats));
        cv::Size thumbnail_size = {mats[0].cols, mats[0].rows};
        super::basic_handle(pack, [&]() {
            for (int i = 0; i < output_size.height; i += thumbnail_size.height) {
                for (int j = 0; j < output_size.width; j += thumbnail_size.width) {
                    cv::Rect roi(cv::Point(j, i), thumbnail_size);
                    cv::Mat symbol = mats[img.at<uchar>(i / thumbnail_size.height, j / thumbnail_size.width) / 4];
                    symbol.copyTo(output_mat(roi));
                }
            }
            printf("進度: %f%%\r", (process++ / super::frame_total) * 100);
            super::type == IMG ? (void)imwrite("output_pic.png", output_mat) : super::writer.write(output_mat);
        });
    }

    void braille() {
        //垂直限制能被4整除，水平限制2整除
        SettingDataPack pack = SettingDataPack::create(param, "collage_output")
                               .set_color(cv::COLOR_BGR2GRAY)
                               .set_dsize("braille", original_video_size, {2, 4});
        int process = 0;
        bool auto_thresh = false;

        cv::Size output_size = {pack.dsize.width * 4, pack.dsize.height * 4};
        cv::Mat output_mat(output_size.height, output_size.width, CV_8UC3);
        vector<vector<char>> braille_string(pack.dsize.width, vector<char>(pack.dsize.height));

        if (pack.thresh == -1) {
            auto_thresh = true;
        }

        if (super::type == IMG)
            pack.dsize = cv::Size(super::img.cols / 4, super::img.rows / 4);
        else
            super::create_written(pack.dsize, output_size);

        auto mats = read_img("font\\braille\\");
        cv::Size thumbnail_size = {mats.begin()->second.cols, mats.begin()->second.rows};

        super::basic_handle(pack, [&]() {
            if (auto_thresh) {
                pack.thresh = mean(super::img).val[0];
            }

            braille_create(braille_string, pack.thresh, pack.dsize);

            for (int i = 0, row = 3; i < output_size.height; i += thumbnail_size.height, row += 4) {
                for (int j = 0, col = 0; j < output_size.width; j += thumbnail_size.width, col++) {
                    string buf = {
                            braille_string[row - 3][col], braille_string[row - 2][col], braille_string[row - 1][col],
                            braille_string[row][col]
                        };
                    cv::Rect roi(cv::Point(j, i), thumbnail_size);
                    cv::Mat symbol = mats[buf + ".png"];
                    symbol.copyTo(output_mat(roi));
                }
            }
            super::type == IMG ? (void)imwrite("output_pic.png", output_mat) : super::writer.write(output_mat);
            printf("進度: %f%%\r", (process++ / super::frame_total) * 100);
        });
    }
};
