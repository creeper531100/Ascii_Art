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

        pack.output_size = {pack.dsize.width * 8, pack.dsize.height * 16};
        cv::Mat output_mat(pack.output_size, CV_8UC3);

        vector<cv::Mat> mats = read_folder_as_list("font\\font\\");
        cv::Size thumbnail_size = {mats[0].cols, mats[0].rows};
        super::basic_handle(pack, [&]() {
            for (int i = 0, row = 0; i < pack.output_size.height; i += thumbnail_size.height, row++) {
                for (int j = 0, col = 0; j < pack.output_size.width; j += thumbnail_size.width, col++) {
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
                               .enable_thresh_detect()
                               .set_dsize("braille", original_size, {8, 16}, {2, 4});
        //輸出解析度放大 480x268 -> 1920x1072
         
        pack.output_size = {pack.dsize.width * (8 / 2), pack.dsize.height * (16 / 4)};
        cv::Mat output_mat(pack.output_size, CV_8UC3);

        vector<vector<char>> braille_string2(pack.dsize.height, vector<char>(pack.dsize.width));
        bool auto_thresh = (pack.thresh == AUTO_DETECT);

        auto mats = read_folder_as_map("font\\braille\\");
        cv::Size thumbnail_size = {mats.begin()->second.cols, mats.begin()->second.rows};

        super::basic_handle(pack, [&]() {
            if (auto_thresh) {
                pack.thresh = mean(super::img).val[0];
            }

            braille_create2(braille_string2, pack.thresh);
            for (int row = 3, i = 0; i < pack.output_size.height; row += 4, i += thumbnail_size.height) {
                for (int col = 0, j = 0; j < pack.output_size.width; col += 1, j += thumbnail_size.width) {
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
        SettingDataPack pack = SettingDataPack::create(param, "collage_output")
                               .set_color(cv::COLOR_BGR2GRAY)
                               .set_output_mode(OutputMode::ORIGIN_SIZE)
                               .enable_thresh_detect();

        cv::Size size = original_size;
        int cap = param["collage_output"]["qt"]["cap"];
        bool have_texture_path = (param["collage_output"]["qt"]["texture"] != "-1");
        bool auto_thresh = (pack.thresh == AUTO_DETECT);

        int reverse = param["collage_output"]["qt"]["reverse"];
        auto thresh_cmp = [&reverse](int val1, int val2) {
            if (reverse) {
                return val1 > val2;
            }
            return val1 < val2;
        };

        Recti boundary(size.width / 2, size.height / 2, size.width / 2, size.height / 2);

        Mat texture;
        if (have_texture_path)
            texture = cv::imread(param["collage_output"]["qt"]["texture"]);

        cv::Mat output_mat(original_size, CV_8UC3);
        super::basic_handle(pack, [&]() {
            Qt qt(boundary, cap);
            if (auto_thresh)
                pack.thresh = mean(super::img).val[0];

            output_mat = cv::Scalar(0, 0, 0);
            for (int i = 0; i < size.height; i++) {
                for (int j = 0; j < size.width; j++) {
                    if (thresh_cmp(img.at<uchar>(i, j), pack.thresh)) {
                        qt.insert({j, i}, cap);
                    }
                }
            }

            if (!have_texture_path) {
                texture = super::orig_img;
            }
            qt.show(output_mat, texture, cap, have_texture_path);
            return &output_mat;
        });
    }
};
