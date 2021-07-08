#pragma once
#include "ImgHandle.h"
#ifndef QUICK_OUTPUT 
#define QUICK_OUTPUT
class QuickOutput : public ImgHandle {
	using super = ImgHandle;
	int count = 0;
	void quick_output_basic_handle(function<void(int*, Mat*, char*, Point*, double*)> puttext_func,
	                               function<Mat()> handle_mat, ColorConversionCodes&& color,
	                               int&& type);

public:
	using super::super;
	void ascii() override;
	void ascii_color();
};
#endif
