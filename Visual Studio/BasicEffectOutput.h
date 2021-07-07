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
	
	void thresholding(vector<int> arg = { THRESH_BINARY , -1, COLOR_BGR2GRAY }) {
		int threshold_type = arg[0];
		int thresh = arg[1];
		int color = arg[2];
		super::type == IMG ? void() : super::video_written_handle();
		super::basic_handle([&]() {
			Mat threshold_mat;
			if (thresh = -1) thresh = mean(super::img).val[0];
			threshold(super::img, threshold_mat, thresh, 255, threshold_type);
			super::type == IMG ? (void)imwrite("output_pic.png", threshold_mat) : super::writer.write(threshold_mat);
			printf("進度: %f%%\r", (this->count++ / super::frame_total) * 100);
		}, ColorConversionCodes(forward<int>(color)));
	}

	void relief(vector<int> arg = { 128 , 1 }) {
		int offset = arg[0];
		int effect = arg[1];
		
		super::type == IMG ? void() : super::video_written_handle();
		super::basic_handle([&]() {
		Mat relief2(super::img.size(), CV_8UC3);
		for (int i = effect; i < super::dsize.area() - effect; i++) {
			for (int j = 0; j < 3; j++) {
				int res2 = this->img.at<Vec3b>(i + effect)[j] - this->img.at<Vec3b>(i - effect)[j] + offset;
				relief2.at<Vec3b>(i)[j] = cv::saturate_cast<uchar>(res2);
			}
		}
		printf("進度: %f%%\r", (this->count++ / super::frame_total) * 100);
		super::type == IMG ? (void)imwrite("output_pic.png", relief2) : super::writer.write(relief2);
		});
	}
};

#endif