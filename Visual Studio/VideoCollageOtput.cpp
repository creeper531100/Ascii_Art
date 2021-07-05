#include "VideoCollageOtput.h"

vector<string> VideoCollageOutput::split(string split_str, string&& delimiter) {
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

void VideoCollageOutput::init_ascii() {
	for (int i = 0; i <= 64; i++)
		this->ascii_mat_arr->at(i) = imread("font\\font\\" + to_string(i) + ".png");
}

void VideoCollageOutput::init_braille() {
	vector<string> file_names;
	glob("font\\braille", file_names);
	for (auto& row : file_names) {
		this->braile_mat_arr[split(row, "\\")[2]] = imread(row);
	}
}

void VideoCollageOutput::ascii(string filename) {
	super::type == IMG ? void() : super::video_written_handle(filename, Size(super::dsize.width * 8, super::dsize.height * 16 - 16));
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
		printf("進度: %f%%\r", (this->count++ / super::frame_total) * 100);
		super::type == IMG ? (void)imwrite("output_pic.png", vertical) : super::writer.write(vertical);
	});
}

void VideoCollageOutput::braille(string filename) {
	super::type == IMG ? void() : super::video_written_handle(filename, Size(super::dsize.width * 4 + 8, super::dsize.height * 4));
	this->init_braille();
	super::gray_ascii_art([&]() {
		Mat vertical, horizontal;
		this->braille_create();
		for (int i = 3; i < this->braille_string->size(); i += 4) {
			horizontal = this->braile_mat_arr["kkkk.png"];
			for (int j = 0; j < super::dsize.height; j++)
				hconcat(horizontal,
					this->braile_mat_arr[this->braille_string->at(i - 3)[j] + this->braille_string->at(i - 2)[j] +
					this->braille_string->at(i - 1)[j] + this->braille_string->at(i)[j] + ".png"], horizontal);
			vertical.push_back(horizontal);
		}
		delete this->braille_string;
		printf("進度: %f%%\r", (this->count++ / super::frame_total) * 100);
		super::type == IMG ? (void)imwrite("output_pic.png", vertical) : super::writer.write(vertical);
	});
}