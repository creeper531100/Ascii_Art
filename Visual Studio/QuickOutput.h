#pragma once
#include "ImgHandle.h"
#ifndef QUICK_OUTPUT 
#define QUICK_OUTPUT
class QuickOutput : public ImgHandle {
	using super = ImgHandle;
	int count = 0;
	void quick_output_basic_handle(function<void(int*, Mat, char*, Point*, double*)> puttext_func,
	                               function<Mat()> handle_mat, vector<int> argv, ColorConversionCodes&& color,
	                               int&& type);

public:
	using super::super;
	void ascii(vector<int> argv = {5});
	void ascii_color(vector<int> argv = {5, 0});
};
#endif
