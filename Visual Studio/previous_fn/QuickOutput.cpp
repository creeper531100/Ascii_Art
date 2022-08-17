#include "QuickOutput.h"

void QuickOutput::quick_output_basic_handle(function<void(int*, Mat*, char*, Point*, double*)> puttext_func,
                                            function<Mat()> handle_mat, ColorConversionCodes&& color, int&& type) {
    double font_size = (double)super::setting_argv["quick_output"]["font_size"] / 10.0;
    int w_enlarge = (int)super::setting_argv["quick_output"]["font_size"] * 2.4; //12
    int h_enlarge = (int)super::setting_argv["quick_output"]["font_size"] * 3; //15
    auto str = new char[2];

    super::type == IMG
        ? void(super::dsize = Size(super::img.cols / 6, super::img.rows / 7))
        : super::video_written_handle(Size(super::dsize.width * w_enlarge, super::dsize.height * h_enlarge));
    Point point;
    Mat mat, this_mat;
    super::basic_handle([&]() {
        mat = Mat(Size(super::dsize.width * w_enlarge, super::dsize.height * h_enlarge), type,
                  USAGE_ALLOCATE_DEVICE_MEMORY);
        this_mat = handle_mat();
        point.y = 0;
        for (int i = 0; i < super::dsize.area(); i++) {
            if (i % super::dsize.width == 0) {
                point.y += h_enlarge;
                point.x = 0;
            }
            else
                point.x += w_enlarge;
            *str = lv[this_mat.at<uchar>(i) / 4] + '\0';
            puttext_func(&i, &mat, str, &point, &font_size);
        }
        printf("進度: %f%%\r", (this->count++ / super::frame_total) * 100);
        super::type == IMG ? (void)imwrite("output_pic.png", mat) : super::writer.write(mat);
    }, forward<ColorConversionCodes>(color));
}

void QuickOutput::ascii() {
    quick_output_basic_handle([&](int* index, Mat* mat, char* str, Point* point, double* font_size) {
                                  putText(*mat, str, *point, FONT_HERSHEY_SIMPLEX, *font_size,
                                          Scalar(255, 255, 255), 1, 8, 0);
                              }, [&]() {
                                  return super::img;
                              },
                              COLOR_BGR2GRAY, CV_8U);
}

void QuickOutput::ascii_color(int sw) {
    string single_char = "";
    single_char += super::setting_argv["quick_output"]["fill_char"];
    bool is_fill = ((single_char[0] == (char)0) || sw);
    quick_output_basic_handle([&](int* index, Mat* mat, char* str, Point* point, double* font_size) {
                                  putText(*mat, is_fill ? str : single_char, *point,
                                          FONT_HERSHEY_SIMPLEX, *font_size,
                                          Scalar(super::img.at<Vec3b>(*index)[0], super::img.at<Vec3b>(*index)[1],
                                                 super::img.at<Vec3b>(*index)[2]), 1, 8, 0);

                              }, [&]() {
                                  Mat m;
                                  cvtColor(super::img, m, COLOR_BGR2GRAY);
                                  return m;

                              }, COLOR_BGR2BGR565, CV_8UC3);
}
