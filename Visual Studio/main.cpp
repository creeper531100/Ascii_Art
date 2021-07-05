#include <iostream>
#include <windows.h>
#include <opencv2/opencv.hpp>

#include "ConsoleShow.h"
#include "VideoCollageOtput.h"

using namespace std;
using namespace cv;

int main() {
	//ConsoleShow mat(VideoCapture("D:\\動漫\\點兔子.mp4"), Size(237, 64));
	//ConsoleShow mat(VideoCapture("D:\\動漫\\廢物天使.mp4"), Size(240, 120));
	//VideoCollageOutput cap(VideoCapture("C:\\Users\\creep\\OneDrive\\桌面\\路邊停車\\Ascii\\範例影片\\test.mp4")//, Size(240, 67));
	string path;
	cin >> path;
	VideoCollageOutput cap(VideoCapture(path), Size(480, 240));
	cap.braille(path);
}


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