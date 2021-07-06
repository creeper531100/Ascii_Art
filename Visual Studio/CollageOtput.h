#pragma once
#include "ImgHandle.h"
#ifndef VIDEO_COLLAGE_OUTPUT 
#define VIDEO_COLLAGE_OUTPUT
class CollageOutput final : public ImgHandle {
private:
	using super = ImgHandle;
	vector<Mat>* ascii_mat_arr = new vector<Mat>(65);
	map<string, Mat> braile_mat_arr;
	int count = 0;
	vector<string> split(string split_str, string&& delimiter);
	void init_ascii();
	void init_braille();
public:
	using super::super;
	void ascii();
	void braille();
};
#endif
