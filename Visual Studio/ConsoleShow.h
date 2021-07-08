#pragma once
#include "ImgHandle.h"
#ifndef CONSOLE_HANDLE 
#define CONSOLE_HANDLE

using namespace std;
using namespace cv;

class ConsoleShow final : public ImgHandle {
private:
	wchar_t* screen;
	HANDLE handle;
	DWORD dwBytesWritten;
	using super = ImgHandle;
	void init();
	void init_word();
	void handle_console(wchar_t** screen, cv::Size& dsize, HANDLE* hConsole);
	void video_interval(chrono::time_point<chrono::system_clock>*);
	map<string, wchar_t> map_pairs;
	
public:
	void ascii(map<string, int> argv) override;
	void braille(map<string, int> argv) override;
	using super::super;
};

#endif 
