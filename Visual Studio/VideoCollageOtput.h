#pragma once
#include "ImgHandle.h"
#ifndef VIDEO_COLLAGE_OUTPUT 
#define VIDEO_COLLAGE_OUTPUT
class VideoCollageOutput final : public ImgHandle {
	
private:
	using super = ImgHandle;
	vector<Mat>* ascii_mat_arr = new vector<Mat>(65);
	map<string, Mat> braile_mat_arr;

	vector<string> split(string split_str, string&& delimiter) { //分割特定字元
		int pos_start = 0, pos_end, delim_len = delimiter.length();
		vector<string> res;

		while ((pos_end = split_str.find(delimiter, pos_start)) != string::npos) {
			string token = split_str.substr(pos_start, pos_end - pos_start);
			pos_start = pos_end + delim_len;
			res.push_back(token);
		}

		res.push_back(split_str.substr(pos_start));
		return res;
	}

	void init_ascii() {
		for (int i = 0; i <= 64; i++)
			this->ascii_mat_arr->at(i) = imread("font\\font\\" + to_string(i) + ".png");
	}

	void init_braille() {
		vector<string> file_names;
		glob("font\\braille", file_names);
		for (auto& row : file_names) {
			this->braile_mat_arr[split(row, "\\")[2]] = imread(row);
		}
	}

public:
	using super::super;
	void ascii_collage_output(string save_path) {
		super::video_written_handle(save_path, Size(super::dsize.width * 8, super::dsize.height * 16 - 16));
		this->init_ascii();
		super::gray_ascii_art([&]() {
			Mat vertical, horizontal;
			for (int i = 0; i < super::dsize.area(); i++) {
				if (i % super::dsize.width == 0) {
					vertical.push_back(horizontal);
					horizontal = this->ascii_mat_arr->at(0);
				}
				else
					hconcat(horizontal, this->ascii_mat_arr->at(super::img.at<uchar>(i) / 4), horizontal);
			}
			super::writer.write(vertical);
		});
		super::writer.release();
	}

};
#endif