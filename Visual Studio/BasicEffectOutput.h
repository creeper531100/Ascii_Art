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
};

#endif