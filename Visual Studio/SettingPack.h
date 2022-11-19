#pragma once
#include "header.h"

struct SettingDataPack {
    cv::Size dsize;
    cv::Size output_size;
    cv::ColorConversionCodes color;
    int thresh;
    Json param;
    std::string func_name;

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

    SettingDataPack& set_output_mode(OutputSizeMode mode) {
        this->output_size.width = (int)mode;
        return *this;
    }

    SettingDataPack& enable_thresh_detect() {
        this->thresh = param[func_name]["thresh"];
        return *this;
    }

    /**
     * \brief �]�w�Y��ؤo
     * \param mode:                  �Ҧ��W��
     * \param original_video_size:   ��l�ؤo
     * \param thumbnail_size:        �Y�Ϥؤo
     * \param zoom:                  ��r�e�פ��
     * \return SettingDataPack&
     */
    SettingDataPack& set_dsize(const char* mode, cv::Size& original_video_size, cv::Size thumbnail_size = {8, 16},
                               std::pair<int, int> zoom = {1, 1}) {
        /*
         * width��8 => �]��img�Qresize�F�A��X�Ϲ������Q�X�R�ܭ�l�ѪR��(thumbnail�Y�ϡA���W�Y�ϧY��l�ؤo)
         * width��zoom => �@�Ӥ�r�e�ڨ��e��
         * width��8�A��8 => �o��O���F�����ѪR�סA����8�h���p�ơA�b��8�^��������l�ѪR��
         * �P�zheight
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

template <typename Concrete>
struct SettingDataPack2 {
    cv::Size dsize; //resize�j�p
    cv::Size output_size; //�v����X�e��
    cv::ColorConversionCodes color; //�C��
    string class_name;

    OutputSizeMode2 output_mode;
    int thresh; //���
    Json param; //�Ѽ�

    SettingDataPack2(Json param) :
        dsize(cv::Size{AUTO_RESIZE}),
        color((cv::ColorConversionCodes)AUTO_DETECT),
        param(param),
        output_size(cv::Size{AUTO_RESIZE}) {
    }

    static Concrete create(Json param) {
        return Concrete(param);
    }

    Concrete& set_color(cv::ColorConversionCodes color) {
        this->color = color;
        return self();
    }

    Concrete& set_dsize(cv::Size dsize) {
        this->dsize = dsize;
        return self();
    }

    Concrete& set_dsize(const char* mode) {
        this->dsize = {param[class_name][mode]["width"], param[class_name][mode]["height"]};
        return self();
    }

    /**
         * \brief �]�w�Y��ؤo
         * \param mode:                  �Ҧ��W��
         * \param original_video_size:   ��l�ؤo
         * \param thumbnail_size:        �Y�Ϥؤo
         * \param zoom:                  ��r�e�פ��
         * \return SettingDataPack&
         */
    Concrete& set_dsize(const char* mode, cv::Size& original_video_size, cv::Size thumbnail_size = {8, 16},
                               std::pair<int, int> zoom = {1, 1}) {
        /*
         * width��8 => �]��img�Qresize�F�A��X�Ϲ������Q�X�R�ܭ�l�ѪR��(thumbnail�Y�ϡA���W�Y�ϧY��l�ؤo)
         * width��zoom => �@�Ӥ�r�e�ڨ��e��
         * width��8�A��8 => �o��O���F�����ѪR�סA����8�h���p�ơA�b��8�^��������l�ѪR��
         * �P�zheight
         */
        this->dsize = {param[class_name][mode]["width"], param[class_name][mode]["height"]};
        bool auto_reszie = this->dsize.width == AUTO_DETECT || this->dsize.height == AUTO_DETECT;
        if (auto_reszie) {
            this->dsize.width = original_video_size.width / (thumbnail_size.width / zoom.first);
            this->dsize.height = original_video_size.height / (thumbnail_size.height / zoom.second);
        }
        if (zoom.first != 1 && auto_reszie) {
            this->dsize.width = (int)(this->dsize.width / thumbnail_size.width) * thumbnail_size.width;
            this->dsize.height = (int)(this->dsize.height / thumbnail_size.height) * thumbnail_size.height;
        }
        return self();
    }

    Concrete& set_output_size(cv::Size dsize) {
        this->output_size = dsize;
        return self();
    }

    Concrete& set_output_mode(OutputSizeMode2 mode) {
        this->output_mode = mode;
        return self();
    }

    int get_thresh() {
        return param[class_name]["thresh"];
    }

    OutputSetting get_mode() {
        return (OutputSetting)param[class_name]["thresh"];
    }

    bool is_auto_thresh() {
        return (OutputSetting)param[class_name]["thresh"] == OutputSetting::AUTO;
    }

    int get_reverse() {
        return param[class_name]["reverse"];
    }

    Concrete& self() { return static_cast<Concrete&>(*this); }
    Concrete& self() const { return static_cast<Concrete const&>(*this); }
};

struct ConsoleShowPack : SettingDataPack2<ConsoleShowPack> {
    ConsoleShowPack(Json param) : SettingDataPack2(param) {
        class_name = "console_show";
    }

    float get_char_width() {
        return param[class_name]["char_width"];
    }
};

struct CollageOutputPack : SettingDataPack2<CollageOutputPack> {
    CollageOutputPack(Json param) : SettingDataPack2(param) {
        class_name = "collage_output";
    }

    string get_qt_texture() {
        string texture = param[class_name]["qt"]["texture"];
        if (texture == "-1")
            return string();
        return texture;
    }

    int get_qt_cap() {
        return param[class_name]["qt"]["cap"];
    }
};
