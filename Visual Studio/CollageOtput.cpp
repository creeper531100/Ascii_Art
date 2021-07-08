#include "CollageOtput.h"

vector<string> CollageOutput::split(string split_str, string&& delimiter) {
	//分割特定字元
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

void CollageOutput::init_ascii() {
	for (int i = 0; i <= 64; i++)
		this->ascii_mat_arr->at(i) = imread("font\\font\\" + to_string(i) + ".png");
}

void CollageOutput::init_braille() {
	vector<string> file_names;
	glob("font\\braille", file_names);
	for (auto& row : file_names) {
		this->braile_mat_arr[split(row, "\\")[2]] = imread(row);
	}
}

void CollageOutput::ascii() {
	super::type == IMG ? void(super::dsize = Size(super::img.cols / 8, super::img.rows / 16)) : super::video_written_handle(Size(super::dsize.width * 8, super::dsize.height * 16 - 16));
	this->init_ascii();
	super::basic_handle([&]() {
		Mat vertical, horizontal;
		for (int i = 0; i < super::dsize.area(); i++) {
			if (i % super::dsize.width == 0) {
				vertical.push_back(horizontal);
				horizontal = this->ascii_mat_arr->at(0);
			}
			else
				hconcat(horizontal, this->ascii_mat_arr->at(super::img.at<uchar>(i) / 4), horizontal);
		}
		printf("進度: %f%%\r", (this->count++ / super::frame_total) * 100);
		super::type == IMG ? (void)imwrite("output_pic.png", vertical) : super::writer.write(vertical);
	}, COLOR_BGR2GRAY);
}

void CollageOutput::braille() {
	super::type == IMG ? void(super::dsize = Size(super::img.cols / 4 , super::img.rows / 4)) : super::video_written_handle(Size(super::dsize.width * 4 + 8, super::dsize.height * 4));
	this->init_braille();
	int thresh = super::setting_argv["collage_output"]["thresh"];
	super::basic_handle([&]() {
		Mat vertical, horizontal;
		if (thresh == -1) thresh = mean(super::img).val[0];
		this->braille_create(thresh);
		for (int i = 3; i < this->braille_string.size(); i += 4) {
			horizontal = this->braile_mat_arr["kkkk.png"];
			for (int j = 0; j < this->braille_string.at(i).size(); j++)
				hconcat(horizontal,
					this->braile_mat_arr[this->braille_string.at(i - 3)[j] + this->braille_string.at(i - 2)[j] +
					this->braille_string.at(i - 1)[j] + this->braille_string.at(i)[j] + ".png"], horizontal);
			vertical.push_back(horizontal);
		}
		super::type == IMG ? (void)imwrite("output_pic.png", vertical) : super::writer.write(vertical);
		printf("進度: %f%%\r", (this->count++ / super::frame_total) * 100);
	}, COLOR_BGR2GRAY);
}