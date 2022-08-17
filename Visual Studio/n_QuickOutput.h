#pragma once
#include "pch.h"
#include "n_Imghandle.h"

class QuickOutput : public ImageHandle {
public:
    using super = ImageHandle;
    using super::super;

    enum Mode { DEFAULT, COLOR, FILLED };

    void ascii(Mode mode) {
        double font_size = (double)super::param["quick_output"]["font_size"] / 10.0f;
        double enlarge = super::param["quick_output"]["font_size"];
        int h_offset = super::param["quick_output"]["h_offset"]; //高度有跑掉請調整這個
        char fill_char = ((string)super::param["quick_output"]["fill_char"]).c_str()[0];
        cv::Size thumbnail_size = {(int)(enlarge * 2.4f), (int)(enlarge * 3.0f)};
        cv::ColorConversionCodes color_conversion_codes = cv::COLOR_BGR2GRAY;

        if (mode != DEFAULT) {
            color_conversion_codes = (cv::ColorConversionCodes)-1;
        }

        SettingDataPack pack = SettingDataPack::create(param, "quick_output")
                               .set_color(color_conversion_codes)
                               .set_dsize("ascii", original_size, thumbnail_size);

        cv::Size output_size = { (pack.dsize.width * thumbnail_size.width), (pack.dsize.height * thumbnail_size.height)};
        cv::Mat output_mat(output_size, CV_8UC3);

        if (super::type == VIDEO) {
            super::create_written(pack.dsize, output_size);
        }

        cv::Scalar scalar = cv::Scalar(255, 255, 255);
        super::basic_handle(pack, [&]() {
            output_mat = cv::Scalar(0, 0, 0);
            for (int i = 0; i < output_size.height; i += thumbnail_size.height) {
                for (int j = 0; j < output_size.width; j += thumbnail_size.width) {
                    char str[2] = {lv[img.at<uchar>(i / thumbnail_size.height, j / thumbnail_size.width) / 4], '\0'};
                    if (mode != DEFAULT) {
                        scalar = img.at<cv::Vec3b>(i / thumbnail_size.height, j / thumbnail_size.width);
                    }
                    if (mode == FILLED) {
                        str[0] = fill_char;
                    }
                    putText(output_mat, str, {j, i + thumbnail_size.height - h_offset }, cv::FONT_HERSHEY_SIMPLEX, font_size, scalar, 1, 8, 0);
                }
            }
            return &output_mat;
        });
    }
};
