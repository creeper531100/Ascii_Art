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