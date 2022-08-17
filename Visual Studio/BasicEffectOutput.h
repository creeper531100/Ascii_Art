#pragma once
#include "n_Imghandle.h"
#include "pch.h"

#ifndef BASIC_EFFECT_OUTPUT
#define BASIC_EFFECT_OUTPUT

class BasicEffectOutput : public ImageHandle {
private:
	using super = ImageHandle;
	int count = 0;
public:
	using super::super;
	void thresholding();
	void relief();
	void math_function_handle(function<double(double*)> math_func);
	void sin_handle();
	void contours_handle(function<cv::Mat*(vector<vector<cv::Point>>*, vector<cv::Vec4i>*)> func);
	void trace();
	void sketch();
};

#endif
