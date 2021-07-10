#include "BasicEffectOutput.h"

void BasicEffectOutput::math_function_handle(function<double(double*)> math_func) {
	super::type == IMG ? void() : super::video_written_handle();
	double deltaI = (double)super::setting_argv["basic_effect"]["math_function_handle"]["deltaI"] / 100;
	int arg = super::setting_argv["basic_effect"]["math_function_handle"]["arg"];
	super::basic_handle([&]() {
		Mat src(this->img.size(), CV_8UC3);
		for (int i = 0; i < src.rows; i++) {
			double x = 0.0;
			for (int j = 0; j < src.cols; j++) {
				int change = (arg * math_func(&x));
				if (i + change < src.rows && i + change > 0) {
					src.at<Vec3b>(i, j) = img.at<Vec3b>(i + change, j);
				}
				x += deltaI;
			}
		}
		printf("進度: %f%%\r", (this->count++ / super::frame_total) * 100);
		super::type == IMG ? (void)imwrite("output_pic.png", src) : super::writer.write(src);
	});
}

void BasicEffectOutput::contours_handle(function<void(vector<vector<Point>>*, vector<Vec4i>*)> func) {
	super::type == IMG ? void() : super::video_written_handle();
	int thresh[2] = { super::setting_argv["basic_effect"]["contours_handle"]["thresh"]["min"], super::setting_argv["basic_effect"]["contours_handle"]["thresh"]["max"] };
	super::basic_handle([&]() {
		Mat drawing(super::img.size(), CV_8UC3);
		super::img.copyTo(drawing);
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		cvtColor(drawing, drawing, COLOR_BGR2GRAY);
		GaussianBlur(drawing, drawing, Size(5, 5), 0);
		if (thresh[0] == -1) {
			int avg = mean(super::img).val[0];
			Canny(drawing, drawing, avg - 10, avg + 10);
		} else {
			Canny(drawing, drawing, thresh[0], thresh[1]);
		}
		findContours(drawing, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		func(&contours, &hierarchy);
		printf("進度: %f%%\r", (this->count++ / super::frame_total) * 100);
	});
}

void BasicEffectOutput::thresholding() {
	super::type == IMG ? void() : super::video_written_handle();
	int thresh = super::setting_argv["basic_effect"]["thresholding"]["thresh"];
	super::basic_handle([&]() {
		Mat threshold_mat;
		if (thresh == -1) {
			int avg = mean(super::img).val[0];
			threshold(super::img, threshold_mat, avg, 255, THRESH_BINARY);
		}
		else
			threshold(super::img, threshold_mat, thresh, 255, THRESH_BINARY);
		super::type == IMG ? (void)imwrite("output_pic.png", threshold_mat) : super::writer.write(threshold_mat);
		printf("進度: %f%%\r", (this->count++ / super::frame_total) * 100);
	}, COLOR_BGR2GRAY);
}

void BasicEffectOutput::relief() {
	super::type == IMG ? void() : super::video_written_handle();
	int effect = super::setting_argv["basic_effect"]["relief"]["effect"];
	int offset = super::setting_argv["basic_effect"]["relief"]["offset"];
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

void BasicEffectOutput::sin_handle() {
	math_function_handle([](double* x) {
		return sin(*x);
	});
}

void BasicEffectOutput::trace() {
	int color[3] = { super::setting_argv["basic_effect"]["contours_handle"]["color"][0],  super::setting_argv["basic_effect"]["contours_handle"]["color"][1], super::setting_argv["basic_effect"]["contours_handle"]["color"][2] };

	contours_handle([&](vector<vector<Point>>* contours, vector<Vec4i>* hierarchy) {
		for (int i = 0; i < contours->size(); i++) {
			drawContours(super::img, *contours, i, Scalar(color[0], color[1], color[2]), 2, LINE_AA, *hierarchy, 0);
		}
		super::type == IMG ? (void)imwrite("output_pic.png", super::img) : super::writer.write(super::img);
	});
}

void BasicEffectOutput::sketch() {
	int color[3] = { super::setting_argv["basic_effect"]["contours_handle"]["color"][0],  super::setting_argv["basic_effect"]["contours_handle"]["color"][1], super::setting_argv["basic_effect"]["contours_handle"]["color"][2] };
	int canvas_color[3] = { super::setting_argv["basic_effect"]["contours_handle"]["canvas_color"][0], super::setting_argv["basic_effect"]["contours_handle"]["canvas_color"][1], super::setting_argv["basic_effect"]["contours_handle"]["canvas_color"][2] };

	contours_handle([&](vector<vector<Point>>* contours, vector<Vec4i>* hierarchy) {
		Mat mm(super::img.size(), CV_8UC3);
		mm.setTo(Scalar(canvas_color[0], canvas_color[1], canvas_color[2]));
		for (int i = 0; i < contours->size(); i++) {
			drawContours(mm, *contours, i, Scalar(color[0], color[1], color[2]), 2, LINE_AA, *hierarchy, 0);
		}
		super::type == IMG ? (void)imwrite("output_pic.png", mm) : super::writer.write(mm);
	});
}
