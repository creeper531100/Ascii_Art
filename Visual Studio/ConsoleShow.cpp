#include <chrono>
#include "ConsoleShow.h"
#include "ImgHandle.h"

void ConsoleShow::init() {
	handle_console(&this->screen, super::dsize, &this->handle);
	this->dwBytesWritten = 0;
}

void ConsoleShow::handle_console(wchar_t** screen, cv::Size& dsize, HANDLE* hConsole) {
	*screen = new wchar_t[dsize.area()];
	*hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(*hConsole);
}

void ConsoleShow::video_interval(chrono::time_point<chrono::system_clock>* c_start) {
	std::this_thread::sleep_until(*c_start + chrono::microseconds(this->frame_interval));
	wstring info = to_wstring(frame_interval);
	SetConsoleTitle(info.c_str());
	*c_start = chrono::system_clock::now();
}

void ConsoleShow::ascii(map<string, int> argv) {
	this->init();
	auto start = chrono::system_clock::now();
	super::basic_handle([&]() {
		for (int i = 0; i < super::dsize.area(); i++) {
			this->screen[i] = super::lv[super::img.at<uchar>(i) / 4];
		}
		WriteConsoleOutputCharacter(this->handle, this->screen, super::dsize.area(), { 0, 0 }, &this->dwBytesWritten);
		this->video_interval(&start);
	}, COLOR_BGR2GRAY);
}

void ConsoleShow::braille(map<string, int> argv) {
	this->init();
	this->init_word();
	int thresh = argv["thresh"];
	auto start = chrono::system_clock::now();
	super::basic_handle([&]() {
		if (thresh == -1) thresh = mean(super::img).val[0];
		this->braille_create(thresh);
		for (int i = 3, pixel = 0; i < this->braille_string.size(); i += 4) {
			for (int j = 0; j < super::dsize.height; j++, pixel++) {
				this->screen[pixel] = this->map_pairs[this->braille_string.at(i - 3)[j] + this->braille_string.at(i - 2)[j] + this->braille_string.at(i - 1)[j] + this->braille_string.at(i)[j]];
			}
		}
		WriteConsoleOutputCharacterW(this->handle, this->screen, super::dsize.area(), { 0, 0 }, &this->dwBytesWritten);
		this->video_interval(&start);
	}, COLOR_BGR2GRAY);
}