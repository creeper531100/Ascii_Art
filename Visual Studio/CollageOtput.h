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
	void ascii(map<string, int> argv) override;
	void braille(map<string, int> argv) override;
public:
	using super::super;
};
#endif
