#pragma once
#include "ImgHandle.h"

#ifndef BASIC_EFFECT_OUTPUT
#define BASIC_EFFECT_OUTPUT

class BasicEffectOutput : public ImgHandle {
private:
	using super = ImgHandle;
	int count = 0;
public:
	using super::super;
	void thresholding();
	void relief();
	void math_function_handle(function<double(double*)> math_func);
	void sin_handle();
	void contours_handle(function<void(vector<vector<Point>>*, vector<Vec4i>*)> func);
	void trace();
	void sketch();
};

#endif
