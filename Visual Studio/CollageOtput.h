#pragma once
#include "ImgHandle.h"
#ifndef VIDEO_COLLAGE_OUTPUT
#define VIDEO_COLLAGE_OUTPUT

class CollageOutput final : public ImgHandle {
private:
	using super = ImgHandle;
	vector<Mat>* ascii_mat_arr = new vector<Mat>(65);
	map<string, Mat> braile_mat_arr;
	int count = 0;
	vector<string> split(string split_str, string&& delimiter);
	void init_ascii();
	void init_braille();
	void ascii() override;
	void braille() override;
public:
	using super::super;
	void xa_ultra() {
		super::type == IMG ? void() : super::video_written_handle();

		super::basic_handle([&]() {
			vector<vector<int>> mat_str(img.rows);
			for (int i = 0; i < img.rows; i++) {
				for (int j = 0; j < img.cols; j++) {
					mat_str[i].push_back(img.at<uchar>(i, j) > 128 ? 0 : 1);
				}
			}

			for (int i = 0; i < img.rows; i++) {
				for (int j = 0; j < img.cols; j++) {
					cout << mat_str[i][j];
				}
				cout << endl;
			}
			
			countSquares(mat_str);
		}, COLOR_BGR2GRAY);
		
	}
	
	void countSquares(vector<vector<int>>& matrix) {
		int rows = matrix.size(), cols = matrix[0].size();
		vector<vector<int>> DP = vector<vector<int>>(rows + 1, vector<int>(cols + 1, 0));
		for (int i = 1, k = 0; i <= rows; i++) {
			for (int j = 1; j <= cols; j++, k++) {
				if (matrix[i - 1][j - 1] == 1) {
					DP[i][j] = 1 + min(min(DP[i][j - 1], DP[i - 1][j]), DP[i - 1][j - 1]);
				}
			}
		}
		
		cout << endl;
		for (int i = 1; i < DP.size(); i++) {
			for(int j = 1; j < DP[i].size(); j++) {
				cout << DP[i][j];
			}
			cout << endl;
		}
		
	}
};
#endif