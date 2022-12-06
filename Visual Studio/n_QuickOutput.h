#pragma once
#include "pch.h"
#include "n_Imghandle.h"

class QuickOutput : public ImageHandle {
public:
    using super = ImageHandle;
    using super::super;

    void ascii(FillMode mode) {
        double enlarge = super::param["quick_output"]["font_size"];
        cv::Size thumbnail_size = {(int)(enlarge * 2.4f), (int)(enlarge * 3.0f)};
        cv::ColorConversionCodes color_conversion_codes = cv::COLOR_BGR2GRAY;

        if (mode != FillMode::DEFAULT) {
            color_conversion_codes = (cv::ColorConversionCodes)AUTO_DETECT;
        }
        
        QuickOutputPack pack = make_pack<QuickOutputPack>(param)
                               .set_color(color_conversion_codes)
                               .set_dsize("ascii", original_size, thumbnail_size);

        double font_size = (double)pack.get_font_size() / 10.0f;
        char fill_char = pack.get_fill_char().c_str()[0];
        char str[2] = { '\0' };

        pack.output_size = {(pack.dsize.width * thumbnail_size.width), (pack.dsize.height * thumbnail_size.height)};
        cv::Mat output_mat(pack.output_size, CV_8UC3);
        cv::Scalar scalar = cv::Scalar(255, 255, 255);

        super::basic_handle2(pack, [&]() {
            output_mat = cv::Scalar(0, 0, 0);
            for (int i = 0; i < pack.output_size.height; i += thumbnail_size.height) {
                for (int j = 0; j < pack.output_size.width; j += thumbnail_size.width) {
                    str[0] = lv[img.at<uchar>(i / thumbnail_size.height, j / thumbnail_size.width) / 4];
                    if (mode != FillMode::DEFAULT) {
                        scalar = img.at<cv::Vec3b>(i / thumbnail_size.height, j / thumbnail_size.width);
                    }
                    if (mode == FillMode::FILLED) {
                        str[0] = fill_char;
                    }
                    putText(output_mat, str, {j, i + thumbnail_size.height - pack.get_h_offset()}, cv::FONT_HERSHEY_SIMPLEX,
                            font_size, scalar, 1, 8, 0);
                }
            }
            return &output_mat;
        });
    }
};
