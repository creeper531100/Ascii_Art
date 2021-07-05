#include "ImgHandle.h"

ImgHandle::ImgHandle(Mat handle, Size dsize) {
	this->img = handle;
	this->dsize = dsize;
	this->type = 0;
}

ImgHandle::ImgHandle(VideoCapture handle, Size dsize) {
	this->video_capture = handle;
	this->dsize = dsize;
	this->type = 1;
	this->frame_FPS = this->video_capture.get(CAP_PROP_FPS);
	this->frame_interval = (1 / this->frame_FPS) * 1000000;
}

void ImgHandle::video_written_handle(string save_path, Size set_size) {
	this->writer = VideoWriter(save_path, this->encoding, this->frame_FPS, set_size);
}

void ImgHandle::basic_handle(ColorConversionCodes&& color) {
	cv::resize(this->img, this->img, this->dsize, 0, 0, INTER_CUBIC);
	cv::cvtColor(this->img, this->img, color);
}

void ImgHandle::braille_create() {
	vector<vector<string>>* vec_len = new vector<vector<string>>(this->dsize.height);
	for(int i = 0; i < this->dsize.height; i++) {
		vector<string>deep(this->dsize.width);
		int avg_color = mean(this->img).val[0];
		for (int j = 1, k = 0; j < this->dsize.width; j += 2, k++) {
			if (this->img.at<uchar>(i, j - 1) > avg_color) {
				if (this->img.at<uchar>(i, j) > avg_color)
					deep.at(k) = "m";
				else
					deep.at(k) = "y";
			}
			else {
				if (this->img.at<uchar>(i, j) > avg_color)
					deep.at(k) = "z";
				else
					deep.at(k) = "k";
			}
		}
		vec_len->at(i) = deep;
	}
	this->braille_string = vec_len;
}

void ImgHandle::gray_ascii_art(function<void()>&& func) {
	if (type == 0) {
		this->basic_handle(COLOR_BGR2GRAY);
		func();
	}
	else {
		while (1) {
			this->video_capture >> this->img;
			if (this->img.empty()) break;
			this->basic_handle(COLOR_BGR2GRAY);
			func();
		}
	}
}

