#pragma once
#include "n_Imghandle.h"
#include "pch.h"
#include "Qt.h"

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

inline unordered_map<string, cv::Mat> read_folder_as_map(string path) {
    unordered_map<string, cv::Mat> vec;
    vector<string> names;
    cv::glob(path, names);
    for (auto& row : names) {
        cv::Mat mat = cv::imread(row);
        string name = split(row, "\\")[2];
        vec[name] = mat;
    }
    return vec;
}

inline vector<cv::Mat> read_folder_as_list(string path) {
    vector<cv::Mat> arr;
    for (int i = 0; i <= 64; i++) {
        cv::Mat mat = cv::imread("font\\font\\" + to_string(i) + ".png");
        arr.push_back(mat);
    }
    return arr;
}

class CollageOutput : public ImageHandle {
public:
    using super = ImageHandle;
    using super::super;

    void ascii() {
        SettingDataPack pack = SettingDataPack::create(param, "collage_output")
                               .set_color(cv::COLOR_BGR2GRAY)
                               .set_dsize("ascii", original_size, {8, 16});

        cv::Size output_size = {pack.dsize.width * 8, pack.dsize.height * 16};
        cv::Mat output_mat(output_size, CV_8UC3);

        if (super::type == VIDEO)
            super::create_written(pack.dsize, output_size);

        vector<cv::Mat> mats = read_folder_as_list("font\\font\\");
        cv::Size thumbnail_size = {mats[0].cols, mats[0].rows};
        super::basic_handle(pack, [&]() {
            for (int i = 0, row = 0; i < output_size.height; i += thumbnail_size.height, row++) {
                for (int j = 0, col = 0; j < output_size.width; j += thumbnail_size.width, col++) {
                    cv::Rect roi(cv::Point(j, i), thumbnail_size);
                    cv::Mat symbol = mats[img.at<uchar>(row, col) / 4];
                    symbol.copyTo(output_mat(roi));
                }
            }
            return &output_mat;
        });
    }

    /*
     *  img_zoom    調整影片放大係數
     *  output_size 調整影片輸出畫質
     */

    void braille() {
        SettingDataPack pack = SettingDataPack::create(param, "collage_output")
                               .set_color(cv::COLOR_BGR2GRAY)
                               .thresh_detect()
                               .set_dsize("braille", original_size, {8, 16}, {2, 4});
        //輸出解析度放大 480x268 -> 1920x1072
        cv::Size output_size = {pack.dsize.width * (8 / 2), pack.dsize.height * (16 / 4)};

        cv::Mat output_mat(output_size, CV_8UC3);
        vector<vector<char>> braille_string2(pack.dsize.height, vector<char>(pack.dsize.width));

        bool auto_thresh = (pack.thresh == -1);

        if (super::type == VIDEO)
            super::create_written(pack.dsize, output_size);

        auto mats = read_folder_as_map("font\\braille\\");
        cv::Size thumbnail_size = {mats.begin()->second.cols, mats.begin()->second.rows};

        super::basic_handle(pack, [&]() {
            if (auto_thresh) {
                pack.thresh = mean(super::img).val[0];
            }

            braille_create2(braille_string2, pack.thresh);
            for (int row = 3, i = 0; i < output_size.height; row += 4, i += thumbnail_size.height) {
                for (int col = 0, j = 0; j < output_size.width; col += 1, j += thumbnail_size.width) {
                    string buf = {
                            braille_string2[row - 3][col], braille_string2[row - 2][col], braille_string2[row - 1][col],
                            braille_string2[row][col]
                        };
                    cv::Rect roi(cv::Point(j, i), thumbnail_size);
                    cv::Mat symbol = mats[buf + ".png"];
                    symbol.copyTo(output_mat(roi));
                }
            }
            return &output_mat;
        });
    }

    void qt() {
        SettingDataPack pack = SettingDataPack::create(param)
                               .set_color(cv::COLOR_BGR2GRAY)
                               .set_dsize(original_size);

        int width = original_size.width;
        int height = original_size.height;
        int cap = param["collage_output"]["qt"]["cap"];
        int offset = param["collage_output"]["qt"]["offset"];
        bool have_texture_path = (param["collage_output"]["qt"]["texture"] != "-1");

        Recti boundary({ width / 2, height / 2 }, width / 2, height / 2);
        Mat texture;

        if(have_texture_path)
            texture = cv::imread(param["collage_output"]["qt"]["texture"]);

        cv::Mat output_mat(original_size, CV_8UC3);
        if (super::type == VIDEO)
            super::create_written(original_size);
        
        super::basic_handle(pack, [&](){
            Qt qt(boundary, cap, offset);
            output_mat = cv::Scalar(0, 0, 0);
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    if (img.at<uchar>(i, j) < 127) {
                        qt.insert({j, i}, cap);
                    }
                }
            }

            if(!have_texture_path) {
                texture = super::orig_img;
            }
            qt.show(output_mat, texture, cap, have_texture_path);
            return &output_mat;
        });
    }
};
