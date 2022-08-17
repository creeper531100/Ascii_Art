#include "ImgHandle.h"

#include <nlohmann/json.hpp>

ImgHandle::ImgHandle(string path, nlohmann::json& argv, Size dsize) {
    this->filename = path;
    vector<string> img_extension = {".jpg", ".JPG", ".png", ".PNG", ".tiff"};
    vector<string> video_extension = {".mp4", ".mp3", ".gif"};
    this->setting_argv = argv;

    auto is_file = [path](vector<string> file_type) {
        return std::find_if(file_type.begin(), file_type.end(), [&](string index) {
            return path.find(index) != std::string::npos;
        }) != file_type.end();
    };

    if (is_file(img_extension)) {
        this->img = imread(path);
        cout << "Video Resize Size: " << this->img.size().height << "x" << this->img.size().width << endl;
        if (dsize.height == -1)
            dsize = this->img.size();
        this->dsize = dsize;
        this->type = IMG;
    }

    else if (is_file(video_extension)) {
        this->cap = VideoCapture(path);
        auto size_i = Size(cap.get(CAP_PROP_FRAME_WIDTH), cap.get(CAP_PROP_FRAME_HEIGHT));
        if (dsize.height == -1)
            dsize = size_i;
        this->frame_FPS = this->cap.get(CAP_PROP_FPS);
        this->frame_total = this->cap.get(CAP_PROP_FRAME_COUNT);
        this->frame_interval = (1 / this->frame_FPS) * 1000000;
        this->video_size = size_i;
        this->dsize = dsize;
        this->type = VIDEO;
    }
    else {
        cerr << "不支援此檔案" << endl;
    }
}

void ImgHandle::braille_create(int brightness_threshold) {
    vector<vector<string>> deep_arr;
    for (int i = 0; i < this->dsize.height; i++) {
        vector<string> deep;
        for (int j = 1; j < this->dsize.width; j += 2) {
            if (this->img.at<uchar>(i, j - 1) > brightness_threshold) {
                if (this->img.at<uchar>(i, j) > brightness_threshold)
                    deep.emplace_back("m");
                else
                    deep.emplace_back("y");
            }
            else {
                if (this->img.at<uchar>(i, j) > brightness_threshold)
                    deep.emplace_back("z");
                else
                    deep.emplace_back("k");
            }
        }
        deep_arr.emplace_back(deep);
    }
    this->braille_string = deep_arr;
}

void ImgHandle::video_written_handle(Size set_size) {
    if (set_size.height == -1)
        set_size = this->dsize;
    cout << "Video Resize Size: " << this->dsize.width << "x" << this->dsize.height << endl;
    cout << "Video Output Size: " << set_size.width << "x" << set_size.height << endl;
    this->run = "ffmpeg -i tempvideo.mp4 -i " + this->filename + " -c copy -map 0:v:0 -map 1:a:0 output_video.mp4";
    this->writer = VideoWriter("tempvideo.mp4", this->encoding, this->frame_FPS, set_size);
}

void ImgHandle::img_handle(ColorConversionCodes& color) {
    cv::resize(this->img, this->img, this->dsize, 0, 0, INTER_CUBIC);
    color == COLOR_BGR2BGR565 ? void() : cv::cvtColor(this->img, this->img, color);
}

void ImgHandle::print_output_info(time_t t_start) {
    system(this->run.c_str());
    int totalTime = difftime(time(NULL), t_start);
    printf("\nused %02d:%02d\n", totalTime / 60, totalTime % 60);
    remove("tempvideo.mp4");
    system("pause");
}

void ImgHandle::basic_handle(function<void()>&& func, ColorConversionCodes&& color) {
    if (type == IMG) {
        this->img_handle(color);
        func();
    }
    else if (type == VIDEO) {
        time_t t_start = time(NULL);
        while (1) {
            this->cap >> this->img;
            if (this->img.empty()) break;
            this->img_handle(color);
            func();
        }
        this->writer.release();
        print_output_info(t_start);
    }
}

void ImgHandle::ascii() {
}

void ImgHandle::braille() {
}
