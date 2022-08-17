#ifdef _DEBUG
#include "BasicEffectOutput.h"
void BasicEffectOutput::math_function_handle(function<double(double*)> math_func) {
    double deltaI = (double)super::param["basic_effect"]["math_function_handle"]["deltaI"] / 100;
    int arg = super::param["basic_effect"]["math_function_handle"]["arg"];

    SettingDataPack pack = SettingDataPack::create(param, "basic_effect")
        .set_dsize(original_size);

    super::type == IMG ? void() : super::create_written(original_size);

    super::basic_handle(pack, [&]() {
        cv::Mat src(this->img.size(), CV_8UC3);
        for (int i = 0; i < src.rows; i++) {
            double x = 0.0;
            for (int j = 0; j < src.cols; j++) {
                int change = (arg * math_func(&x));
                if (i + change < src.rows && i + change > 0) {
                    src.at<cv::Vec3b>(i, j) = img.at<cv::Vec3b>(i + change, j);
                }
                x += deltaI;
            }
        }
        return &src;
    });
}

void BasicEffectOutput::contours_handle(function<cv::Mat*(vector<vector<cv::Point>>*, vector<cv::Vec4i>*)> func) {
    SettingDataPack pack = SettingDataPack::create(param, "basic_effect")
                           .set_color(cv::COLOR_BGR2GRAY)
                           .set_dsize(original_size);

    super::type == IMG ? void() : super::create_written(original_size);
    int thresh[2] = {
            super::param["basic_effect"]["contours_handle"]["thresh"]["min"],
            super::param["basic_effect"]["contours_handle"]["thresh"]["max"]
        };

    super::basic_handle(pack, [&]() {
        cv::Mat drawing(super::img.size(), CV_8UC3);
        super::img.copyTo(drawing);
        vector<vector<cv::Point>> contours;
        vector<cv::Vec4i> hierarchy;
        //cvtColor(drawing, drawing, cv::COLOR_BGR2GRAY);
        GaussianBlur(drawing, drawing, cv::Size(5, 5), 0);
        if (thresh[0] == -1) {
            int avg = mean(super::img).val[0];
            Canny(drawing, drawing, avg - 10, avg + 10);
        }
        else {
            Canny(drawing, drawing, thresh[0], thresh[1]);
        }
        findContours(drawing, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        return func(&contours, &hierarchy);
    });
}

void BasicEffectOutput::thresholding() {
    SettingDataPack pack = SettingDataPack::create(param, "basic_effect")
                           .set_color(cv::COLOR_BGR2GRAY)
                           .set_dsize("thresholding", original_size)
                           .enable_thresh_detect();

    super::type == IMG ? void() : super::create_written(original_size);
    bool auto_thresh = false;
    if (pack.thresh == -1) {
        auto_thresh = true;
    }

    super::basic_handle(pack, [&]() {
        cv::Mat threshold_mat;
        if (auto_thresh) {
            pack.thresh = mean(super::img).val[0];
        }
        threshold(super::img, threshold_mat, pack.thresh, 255, cv::THRESH_BINARY);
        return &threshold_mat;
    });
}

void BasicEffectOutput::relief() {
    SettingDataPack pack = SettingDataPack::create(param, "basic_effect")
        .set_dsize(original_size);

    super::type == IMG ? void() : super::create_written(original_size);
    int effect = super::param["basic_effect"]["relief"]["effect"];
    int offset = super::param["basic_effect"]["relief"]["offset"];
    super::basic_handle(pack, [&]() {
        cv::Mat relief2(super::img.size(), CV_8UC3);
        for (int i = effect; i < pack.dsize.area() - effect; i++) {
            for (int j = 0; j < 3; j++) {
                int res2 = this->img.at<cv::Vec3b>(i + effect)[j] - this->img.at<cv::Vec3b>(i - effect)[j] + offset;
                relief2.at<cv::Vec3b>(i)[j] = cv::saturate_cast<uchar>(res2);
            }
        }
        return &relief2;
    });
}

void BasicEffectOutput::sin_handle() {
    math_function_handle([](double* x) {
        return sin(*x);
    });
}

void BasicEffectOutput::trace() {
    cv::Scalar color = {
            super::param["basic_effect"]["contours_handle"]["color"][0],
            super::param["basic_effect"]["contours_handle"]["color"][1],
            super::param["basic_effect"]["contours_handle"]["color"][2]
        };

    contours_handle([&](vector<vector<cv::Point>>* contours, vector<cv::Vec4i>* hierarchy) {
        for (int i = 0; i < contours->size(); i++) {
            drawContours(super::img, *contours, i, color, 2, cv::LINE_AA, *hierarchy, 0);
        }
        cv::cvtColor(img, img, cv::COLOR_GRAY2BGR);
        return &img;
    });
}

void BasicEffectOutput::sketch() {
    cv::Scalar color = {
            super::param["basic_effect"]["contours_handle"]["color"][0],
            super::param["basic_effect"]["contours_handle"]["color"][1],
            super::param["basic_effect"]["contours_handle"]["color"][2]
        };
    cv::Scalar canvas_color = {
            super::param["basic_effect"]["contours_handle"]["canvas_color"][0],
            super::param["basic_effect"]["contours_handle"]["canvas_color"][1],
            super::param["basic_effect"]["contours_handle"]["canvas_color"][2]
        };

    contours_handle([&](vector<vector<cv::Point>>* contours, vector<cv::Vec4i>* hierarchy) {
        cv::Mat mm(super::img.size(), CV_8UC3);
        mm.setTo(canvas_color);
        for (int i = 0; i < contours->size(); i++) {
            drawContours(mm, *contours, i, color, 2, cv::LINE_AA, *hierarchy, 0);
        }
        return &mm;
    });
}
#endif