#include <iostream>
#include <windows.h>
#include <opencv2/opencv.hpp>

#include "ConsoleShow.h"
#include "CollageOtput.h"
#include "BasicEffectOutput.h"

using namespace std;
using namespace cv;

int main() {
	int sw = 0;
	string path = "";
	int setting = -1;

	cout << "AsciiArt新藝術\n----------------------------------\n(0).預覽 AsciiArt (1).預覽 BrailleArt \n(2).輸出 AsciiArt (3).輸出 BrailleArt \n(4).輸出帥臉 (5).輸出浮雕照\n----------------------------------\n選擇功能: "; cin >> sw;
	cout << "輸入網址或圖片位置: "; cin >> path;
	if (path.find("https") != std::string::npos) {
		remove("HTC.mp4");
		system(("youtube-dl -o HTC.%(ext)s -f mp4 " + path).c_str());
		path = "HTC.mp4";
	}
	
	ImgHandle* img_handle;
		
	switch (sw) {
	case 0:
		img_handle = new ConsoleShow(forward<string>(path), Size(237, 64));
		img_handle->ascii();
		break;
	case 1:
		img_handle = new ConsoleShow(forward<string>(path), Size(240, 120));
		img_handle->braille();
		break;
	case 2:
		img_handle = new CollageOutput(forward<string>(path), Size(240, 67));
		img_handle->ascii();
		break;
	case 3:
		img_handle = new CollageOutput(forward<string>(path), Size(480, 240));
		img_handle->braille();
		break;
	case 4:
		img_handle = new BasicEffectOutput(forward<string>(path));
		img_handle->call_obj_member<BasicEffectOutput>(&BasicEffectOutput::thresholding, vector<int>(2) = { THRESH_BINARY, -1 });
		break;
	case 5:
		img_handle = new BasicEffectOutput(forward<string>(path));
		img_handle->call_obj_member<BasicEffectOutput>(&BasicEffectOutput::relief, vector<int>(2) = { 128, 1 });
		break;
	default: 
		cout << "很遺憾失敗了" << endl;
	}
}