#pragma once
#include <iostream>
#include <windows.h>
#include <opencv2/opencv.hpp>
#ifndef IMG_HANDLE 
#define IMG_HANDLE

using namespace std;
using namespace cv;

void handle_console(wchar_t**, cv::Size&, HANDLE*);
vector<vector<string>> braille_create(Size& , Mat);


class ImgHandle abstract {
private:
	int type;
	void basic_handle(ColorConversionCodes&& color);
protected:
	Mat img;
	void braille_create();
	vector<vector<string>> *braille_handle;
	VideoCapture video_capture;
	Size dsize;
	string lv = " .'`^,:;l!i><~+_--?][}{)(|/rxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
public:
	ImgHandle(Mat, Size);
	ImgHandle(VideoCapture, Size);
	void gray_ascii_art(function<void()>&&);
};


class ConsoleShow : public ImgHandle {
private:
	wchar_t* screen;
	HANDLE handle;
	DWORD dwBytesWritten;
	using super = ImgHandle;
	void init();
	void init_word();
	map<string, wchar_t> map_pairs;
public:
	void ascii();
	void braille();
	ConsoleShow(const Mat& mat, const Size& size2_i);
	ConsoleShow(const VideoCapture& video_capture, const Size& size2_i);
};

class VideoOutput : public ImgHandle {
public:
	VideoOutput(const Mat& mat, const Size& size2_i)
		: ImgHandle(mat, size2_i) {
	}

	VideoOutput(const VideoCapture& video_capture, const Size& size2_i)
		: ImgHandle(video_capture, size2_i) {
	}
};

#endif