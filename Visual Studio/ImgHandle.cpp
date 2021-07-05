#include "ImgHandle.h"

void handle_console(wchar_t** screen, cv::Size& dsize, HANDLE* hConsole) {
	*screen = new wchar_t[dsize.area()];
	*hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(*hConsole);
}

void ImgHandle::basic_handle(ColorConversionCodes&& color) {
	cv::resize(this->img, this->img, this->dsize, 0, 0, INTER_CUBIC);
	cv::cvtColor(this->img, this->img, color);
}

void ImgHandle::braille_create() {
	vector<vector<string>> deep_arr;
	for (int i = 0; i < this->dsize.height; i++) {
		vector<string> deep;
		Scalar avg = mean(this->img);
		int avg_color = avg.val[0];
		for (int j = 1; j < this->dsize.width; j += 2) {
			if (this->img.at<uchar>(i, j - 1) > avg_color) {
				if (this->img.at<uchar>(i, j) > avg_color)
					deep.emplace_back("m");
				else
					deep.emplace_back("y");
			}
			else {
				if (this->img.at<uchar>(i, j) > avg_color)
					deep.emplace_back("z");
				else
					deep.emplace_back("k");
			}
		}
		deep_arr.emplace_back(deep);
	}
	this->braille_handle = deep_arr;
}

void ImgHandle::gray_ascii_art(function<void()> func) {
	if (type == 0) {
		this->basic_handle(COLOR_BGR2GRAY);
		func();
	}
	else {
		while (1) {
			this->video_capture >> this->img;
			this->basic_handle(COLOR_BGR2GRAY);
			func();
		}
	}
}

ImgHandle::ImgHandle(Mat handle, Size dsize) {
	this->img = handle;
	this->dsize = dsize;
	this->type = 0;
}

ImgHandle::ImgHandle(VideoCapture handle, Size dsize) {
	this->video_capture = handle;
	this->dsize = dsize;
	this->type = 1;
}
