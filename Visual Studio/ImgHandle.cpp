#include "ImgHandle.h"

ImgHandle::ImgHandle(Mat img, Size dsize) {
	this->img = img;
	this->dsize = dsize;
	this->type = IMG;
}

ImgHandle::ImgHandle(VideoCapture cap, Size dsize) {
	this->cap = cap;
	this->dsize = dsize;
	this->type = VIDEO;
	this->frame_FPS = this->cap.get(CAP_PROP_FPS);
	this->frame_total = this->cap.get(CAP_PROP_FRAME_COUNT);
	this->frame_interval = (1 / this->frame_FPS) * 1000000;
}

void ImgHandle::video_written_handle(string filename, Size set_size) {
	this->filename = filename;
	this->run = "ffmpeg -i tempvideo.mp4 -i " + this->filename + " -c copy -map 0:v:0 -map 1:a:0 output_video.mp4";
	this->writer = VideoWriter("tempvideo.mp4", this->encoding, this->frame_FPS, set_size);
}

void ImgHandle::basic_handle(ColorConversionCodes&& color) {
	cv::resize(this->img, this->img, this->dsize, 0, 0, INTER_CUBIC);
	cv::cvtColor(this->img, this->img, color);
}

void ImgHandle::braille_create() {
	vector<vector<string>>* vec_len = new vector<vector<string>>(this->dsize.height);
	for(int i = 0; i < this->dsize.height; i++) {
		vector<string>deep(this->dsize.width);
		int avg_color = mean(this->img).val[0];
		for (int j = 1, k = 0; j < this->dsize.width; j += 2, k++) {
			if (this->img.at<uchar>(i, j - 1) > avg_color) {
				if (this->img.at<uchar>(i, j) > avg_color)
					deep.at(k) = "m";
				else
					deep.at(k) = "y";
			}
			else {
				if (this->img.at<uchar>(i, j) > avg_color)
					deep.at(k) = "z";
				else
					deep.at(k) = "k";
			}
		}
		vec_len->at(i) = deep;
	}
	this->braille_string = vec_len;
}


void ImgHandle::print_output_info(time_t t_start) {
	system(this->run.c_str());
	int totalTime = difftime(time(NULL), t_start);
	printf("\nused %02d:%02d\n", totalTime / 60, totalTime % 60);
	remove("tempvideo.mp4");
}

void ImgHandle::gray_ascii_art(function<void()>&& func) {
	if (type == IMG) {
		this->basic_handle(COLOR_BGR2GRAY);
		func();
	}
	else if (type == VIDEO){
		time_t t_start = time(NULL);
		while (1) {
			this->cap >> this->img;
			if (this->img.empty()) break;
			this->basic_handle(COLOR_BGR2GRAY);
			func();
		}
		this->writer.release();
		print_output_info(t_start);
	}

}

