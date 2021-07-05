#pragma once
#include <iostream>
#include <windows.h>
#include <opencv2/opencv.hpp>
#ifndef IMG_HANDLE 
#define IMG_HANDLE

using namespace std;
using namespace cv;

class ImgHandle abstract {
private:
	int type;
	int encoding = VideoWriter::fourcc('D', 'I', 'V', 'X');
	void basic_handle(ColorConversionCodes&& color);
protected:
	VideoWriter writer;
	Mat img;
	double frame_FPS;
	int frame_interval;
	void braille_create();
	vector<vector<string>> *braille_string;
	VideoCapture video_capture;
	Size dsize;
	string lv = " .'`^,:;l!i><~+_--?][}{)(|/rxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
public:
	ImgHandle(Mat, Size);
	ImgHandle(VideoCapture, Size);
	void video_written_handle(string save_path, Size set_size);
	void gray_ascii_art(function<void()>&&);
};
#endif