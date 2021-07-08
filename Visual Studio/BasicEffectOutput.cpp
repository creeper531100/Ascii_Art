#include "BasicEffectOutput.h"

void BasicEffectOutput::thresholding() {
	super::type == IMG ? void() : super::video_written_handle();
	int thresh = super::setting_argv["basic_effect"]["thresh"];
	super::basic_handle([&]() {
		Mat threshold_mat;
		if (thresh == -1) thresh = mean(super::img).val[0];
		threshold(super::img, threshold_mat, thresh, 255, THRESH_BINARY);
		super::type == IMG ? (void)imwrite("output_pic.png", threshold_mat) : super::writer.write(threshold_mat);
		printf("進度: %f%%\r", (this->count++ / super::frame_total) * 100);
	}, COLOR_BGR2GRAY);
}

void BasicEffectOutput::relief() {
	super::type == IMG ? void() : super::video_written_handle();
	int effect = super::setting_argv["basic_effect"]["effect"];
	int offset = super::setting_argv["basic_effect"]["offset"];
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
