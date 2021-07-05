/*void ImgHandle::braille_create() {
	vector<vector<string>> deep_arr;
	for (int i = 0; i < this->dsize.height; i++) {
		vector<string> deep;
		Scalar avg = mean(this->img);
		int avg_color = avg.val[0];
		for (int j = 1; j < this->dsize.width; j += 2) {
			if (this->img.at<uchar>(i, j - 1) > avg_color) {
				if (this->img.at<uchar>(i, j) > avg_color)
					deep.emplace_back("m");
				else
					deep.emplace_back("y");
			}
			else {
				if (this->img.at<uchar>(i, j) > avg_color)
					deep.emplace_back("z");
				else
					deep.emplace_back("k");
			}
		}
		deep_arr.emplace_back(deep);
	}
	this->braille_handle = deep_arr;
}*/