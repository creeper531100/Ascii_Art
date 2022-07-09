#include <fstream>
#include "pch.h"
#include "n_ConsoleShow.h"

using namespace std;
using Json = nlohmann::json;
using namespace cv;

class CollageOutput : public ImageHandle {
public:
    using ImageHandle::ImageHandle;
    using super = ImageHandle;

    vector<Mat> read_img(string path, int size) {
        vector<Mat> mats(size);
        for (int i = 0; i < size; i++)
            mats[i] = imread(path + to_string(i) + ".png");
        return mats;
    }

    void ascii() {
        int width = super::param["collage_output"]["ascii"]["width"];
        int height = super::param["collage_output"]["ascii"]["height"];
        int process = 0;

        Size output_size = {width * 8, height * 16};
        Mat mat(output_size.height, output_size.width, CV_8UC3);
        SettingDataPack pack = SettingDataPack::create().set_color(COLOR_BGR2GRAY).set_dsize({width, height});

        if (super::type == IMG)
            pack.dsize = Size(super::img.cols / 8, super::img.rows / 16);
        else
            super::create_written(pack.dsize, output_size);

        vector<Mat> mats = read_img("font\\font\\", 65);
        Size thumbnail_size = {mats[0].cols, mats[0].rows};
        super::basic_handle(pack, [&]() {
            for (int i = 0; i < output_size.width; i += thumbnail_size.width) {
                for (int j = 0; j < output_size.height; j += thumbnail_size.height) {
                    Rect roi(Point(i, j), thumbnail_size);
                    Mat symbol = mats[img.at<uchar>(j / thumbnail_size.height, i / thumbnail_size.width) / 4];
                    symbol.copyTo(mat(roi));
                }
            }
            printf("進度: %f%%\r", (process++ / super::frame_total) * 100);
            super::type == IMG ? (void)imwrite("output_pic.png", mat) : super::writer.write(mat);
        });
    }
};

int main() {
    int sw = 0;
    string path = "";
    Json json_file;
    ifstream in("setting.json");
    in >> json_file;

    cout <<
        "AsciiArt新藝術\n----------------------------------\n(0).預覽 AsciiArt (1).預覽 BrailleArt \n(2).輸出 AsciiArt (3).輸出 BrailleArt \n(4).輸出帥臉 (5).輸出浮雕照\n(6).快速輸出 AsciiArt (7).輸出 AsciiArt 彩色版 (8).輸出 AsciiArt 彩色版(固定文字)\n(9).函數處理 (10).素描 (11).描邊\n----------------------------------\n選擇功能: ";
    //cin >> sw;
    cout << "輸入網址或圖片位置: ";
    cin >> path;
    if (path.find("https") != std::string::npos) {
        remove("HTC.mp4");
        system(("youtube-dl -o HTC.%(ext)s -f mp4 " + path).c_str());
        path = "HTC.mp4";
    }

    CollageOutput console_shows(path, json_file);
    console_shows.ascii();
}

/*
int main() {
    int sw = 0;
    string path = "";
    json json_file;
    ifstream in("setting.json");
    in >> json_file;

    cout <<
        "AsciiArt新藝術\n----------------------------------\n(0).預覽 AsciiArt (1).預覽 BrailleArt \n(2).輸出 AsciiArt (3).輸出 BrailleArt \n(4).輸出帥臉 (5).輸出浮雕照\n(6).快速輸出 AsciiArt (7).輸出 AsciiArt 彩色版 (8).輸出 AsciiArt 彩色版(固定文字)\n(9).函數處理 (10).素描 (11).描邊\n----------------------------------\n選擇功能: ";
    cin >> sw;
    cout << "輸入網址或圖片位置: ";
    cin >> path;
    if (path.find("https") != std::string::npos) {
        remove("HTC.mp4");
        system(("youtube-dl -o HTC.%(ext)s -f mp4 " + path).c_str());
        path = "HTC.mp4";
    }

    ImgHandle* img_handle = nullptr;
    switch (sw) {
    case 0:
        img_handle = new ConsoleShow(path, json_file,
                                     Size(json_file["console_show"]["ascii"]["width"],
                                          json_file["console_show"]["ascii"]["height"]));
        img_handle->ascii();
        break;
    case 1:
        img_handle = new ConsoleShow(path, json_file,
                                     Size(json_file["console_show"]["braille"]["width"],
                                          json_file["console_show"]["braille"]["height"]));
        img_handle->braille();
        break;
    case 2:
        img_handle = new CollageOutput(path, json_file,
                                       Size(json_file["collage_output"]["ascii"]["width"],
                                            json_file["collage_output"]["ascii"]["height"])); //8x16
        img_handle->ascii();
        break;
    case 3:
        img_handle = new CollageOutput(path, json_file,
                                       Size(json_file["collage_output"]["braille"]["width"],
                                            json_file["collage_output"]["braille"]["height"])); //4 , 480x240
        img_handle->braille();
        break;
    case 4:
        img_handle = new BasicEffectOutput(path, json_file); //1x1
        img_handle->call_obj_member<BasicEffectOutput>(&BasicEffectOutput::thresholding);
        break;
    case 5:
        img_handle = new BasicEffectOutput(path, json_file); //1x1
        img_handle->call_obj_member<BasicEffectOutput>(&BasicEffectOutput::relief);
        break;
    case 6:
        img_handle = new QuickOutput(path, json_file,
                                     Size(json_file["quick_output"]["ascii"]["width"],
                                          json_file["quick_output"]["ascii"]["height"])); //12x15
        img_handle->ascii();
        break;
    case 7:
        img_handle = new QuickOutput(path, json_file,
                                     Size(json_file["quick_output"]["ascii"]["width"],
                                          json_file["quick_output"]["ascii"]["height"])); //12x15
        img_handle->call_obj_member<QuickOutput>(&QuickOutput::ascii_color, 1);
        break;
    case 8:
        img_handle = new QuickOutput(path, json_file,
                                     Size(json_file["quick_output"]["ascii"]["width"],
                                          json_file["quick_output"]["ascii"]["height"])); //12x15
        img_handle->call_obj_member<QuickOutput>(&QuickOutput::ascii_color, 0);
        break;
    case 9:
        img_handle = new BasicEffectOutput(path, json_file); //1x1
        img_handle->call_obj_member<BasicEffectOutput>(&BasicEffectOutput::sin_handle);
        break;
    case 10:
        img_handle = new BasicEffectOutput(path, json_file);
        img_handle->call_obj_member<BasicEffectOutput>(&BasicEffectOutput::sketch);
        break;
    case 11:
        img_handle = new BasicEffectOutput(path, json_file);
        img_handle->call_obj_member<BasicEffectOutput>(&BasicEffectOutput::trace);
        break;
    case 12:
        img_handle = new CollageOutput(path, json_file);
        img_handle->call_obj_member<CollageOutput>(&CollageOutput::xa_ultra);
        break;

    default:
        cout << "很遺憾失敗了" << endl;
    }
    system("pause");
    delete img_handle;
}
*/
