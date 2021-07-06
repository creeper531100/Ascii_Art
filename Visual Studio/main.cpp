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

	cout << "AsciiArt新藝術\n----------------------------------\n(0).預覽AsciiArt, (1).預覽BrailleArt \n(2).輸出AsciiArt, (3).輸出BrailleArt \n(4).輸出帥臉, (5).輸出浮雕照\n----------------------------------\n選擇功能: "; cin >> sw;
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
	}
}

//ConsoleShow mat(VideoCapture("D:\\動漫\\點兔子.mp4"), Size(237, 64));
//ConsoleShow mat(VideoCapture("D:\\動漫\\廢物天使.mp4"), Size(240, 120));
//CollageOutput cap(VideoCapture("C:\\Users\\creep\\OneDrive\\桌面\\路邊停車\\Ascii\\範例影片\\test.mp4")//, Size(240, 67)); //480, 240

/*
 * string folder_path;
	cout << "Path:";
	cin >> folder_path;
	vector<string> file_names;
	vector<cv::Mat> mat_arr;
	cv::glob(folder_path, file_names);
	int file_index = 0;
	for (auto row : file_names) {
		string path = folder_path + "\\this\\" + to_string(file_index++);
		system(("mkdir -p " + path).c_str());
		cv::Mat frame = cv::imread(row);
		cv::VideoWriter write(path + "\\video.mp4", cv::VideoWriter::fourcc('D', 'I', 'V', 'X'), 15, frame.size());
		for (int i = 0; i < 256; i++) {
			cv::Mat cv_mat;
			cv::cvtColor(frame, cv_mat, cv::COLOR_BGR2GRAY);
			threshold(cv_mat, cv_mat, i, 255, cv::THRESH_BINARY);
			printf("%d / %f%%\n", file_index, ((double)i / 256) * 100);
			cv::putText(cv_mat, to_string(i), cv::Point(5, 15), cv::FONT_HERSHEY_SIMPLEX, 0.5, 128, 1, 8, 0);
			cv::imwrite(path + "\\" + to_string(i) + ".PNG", cv_mat);
			write.write(cv_mat);
		}
		write.release();
	}
 */