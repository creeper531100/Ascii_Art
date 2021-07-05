#include "ImgHandle.h"

void ConsoleShow::init() {
	handle_console(&this->screen, super::dsize, &this->handle);
	this->dwBytesWritten = 0;
}

void ConsoleShow::ascii() {
	super::gray_ascii_art([&]() {
		for (int i = 0; i < super::dsize.area(); i++) {
			this->screen[i] = super::lv[super::img.at<uchar>(i) / 4];
		}
		WriteConsoleOutputCharacter(this->handle, this->screen, super::dsize.area(), { 0, 0 }, &this->dwBytesWritten);
	});
}

void ConsoleShow::braille() {
	this->init_word();
	super::gray_ascii_art([&]() {
		this->braille_create();
		wstring basic_string;
		for (int i = 3; i < this->braille_handle.size(); i += 4) {
			for (int j = 0; j < super::dsize.height; j++) {
				basic_string.push_back(this->map_pairs[this->braille_handle[i - 3][j] + this->braille_handle[i - 2][j] + this->braille_handle[i - 1][j] + this->braille_handle[i][j]]);
			}
		}
		WriteConsoleOutputCharacterW(this->handle, basic_string.c_str(), super::dsize.area(), { 0, 0 }, &this->dwBytesWritten);
	});
}

ConsoleShow::ConsoleShow(const Mat& mat, const Size& size2_i):
ImgHandle(mat, size2_i), screen(nullptr), handle(nullptr), dwBytesWritten(0) {
	this->init();
}

ConsoleShow::ConsoleShow(const VideoCapture& video_capture, const Size& size2_i):
ImgHandle(video_capture, size2_i), screen(nullptr), handle(nullptr), dwBytesWritten(0) {
	this->init();
}

