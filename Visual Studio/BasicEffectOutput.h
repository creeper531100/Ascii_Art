#pragma once
#include "ImgHandle.h"

#ifndef BASIC_EFFECT_OUTPUT
#define BASIC_EFFECT_OUTPUT

class BasicEffectOutput : public ImgHandle {
private:
	using super = ImgHandle;
public:
	using super::super;
	
	void thresholding_output(ThresholdTypes threshold_type, int brightness_threshold = -1) {
		super::type == IMG ? void() : super::video_written_handle();
		super::gray_ascii_art([&]() {
			Mat threshold_mat;
			if (brightness_threshold = -1) brightness_threshold = mean(this->img).val[0];
			threshold(this->img, threshold_mat, brightness_threshold, 255, threshold_type);
			super::type == IMG ? (void)imwrite("output_pic.png", threshold_mat) : super::writer.write(threshold_mat);
		});
	}
};

#endif