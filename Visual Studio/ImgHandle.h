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
	int encoding = VideoWriter::fourcc('D', 'I', 'V', 'X');
	void basic_handle(ColorConversionCodes&& color);
protected:
	enum varType {
		IMG,
		VIDEO
	};
	varType type;
	VideoWriter writer;
	Mat img;
	string filename;
	double frame_FPS;
	double frame_total;
	int frame_interval;
	void braille_create();
	vector<vector<string>> *braille_string;
	VideoCapture cap;
	string run;
	Size dsize;
	string lv = " .'`^,:;l!i><~+_--?][}{)(|/rxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
public:
	ImgHandle(Mat, Size);
	ImgHandle(VideoCapture, Size);
	void video_written_handle(string filename, Size set_size);
	void print_output_info(time_t t_start);
	void gray_ascii_art(function<void()>&&);
};
#endif