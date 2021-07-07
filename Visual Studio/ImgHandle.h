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
	void img_handle(ColorConversionCodes& color);
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
	Size video_size;
	//void braille_create_disable(int&&);
	//vector<vector<string>> *braille_string_disable;


	vector<vector<string>> braille_string;
	void braille_create(int&& brightness_threshold) {
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
	
	VideoCapture cap;
	string run;
	string lv = " .'`^,:;l!i><~+_--?][}{)(|/rxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
	void video_written_handle(Size set_size = Size(-1, -1));
	void print_output_info(time_t);
	void basic_handle(function<void()>&&, ColorConversionCodes&& color = COLOR_BGR2BGR565);
public:
	virtual ~ImgHandle() = default;
	virtual void ascii();
	virtual void braille();
	ImgHandle(string&& , Size dsize = Size(-1, -1));
	Size dsize;

	template<typename T>
	void call_obj_member(void (T::* mem)(vector<int>), vector<int> argv) {
		T* t = new T(forward<string>(this->filename), this->dsize);
		(t->*mem)(argv);
	}
};

#endif

/*
 * void call_obj_member(void (T::* mem)(), Size dsize = Size(-1, -1)) {
		mem_fn(mem)(t);
		t->dsize = dsize;
	}
 */