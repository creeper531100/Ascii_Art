#include <fstream>
#include "pch.h"
#include "n_ConsoleShow.h"

using namespace std;
using Json = nlohmann::json;
using namespace cv;

vector<string> split(string split_str, string&& delimiter) {
    //分割特定字元
    int pos_start = 0, pos_end, delim_len = delimiter.length();
    vector<string> res;

    while ((pos_end = split_str.find(delimiter, pos_start)) != string::npos) {
        string token = split_str.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(split_str.substr(pos_start));
    return res;
}

class CollageOutput : public ImageHandle {
public:
    using ImageHandle::ImageHandle;
    using super = ImageHandle;

    unordered_map<string, Mat> read_img(string path, vector<Mat>&& mats = {}) {
        unordered_map<string, Mat> vec;
        vector<string> names;
        glob(path, names);
        for (auto& row : names) {
            Mat mat = imread(row);
            string name = split(row, "\\")[2];
            vec[name] = mat;
            mats.push_back(mat);
        }
        return vec;
    }

    void ascii() {
        SettingDataPack pack = SettingDataPack::create(param, "collage_output")
                            .set_color(COLOR_BGR2GRAY)
                            .set_dsize("ascii");
        int process = 0;
        Size output_size = { pack.dsize.width * 8, pack.dsize.height * 16};
        Mat output_mat(output_size.height, output_size.width, CV_8UC3);

        if (super::type == IMG)
            pack.dsize = Size(super::img.cols / 8, super::img.rows / 16);
        else
            super::create_written(pack.dsize, output_size);

        vector<Mat> mats;
        read_img("font\\font\\", std::move(mats));
        Size thumbnail_size = {mats[0].cols, mats[0].rows};
        super::basic_handle(pack, [&]() {
            for (int i = 0; i < output_size.height; i += thumbnail_size.height) {
                for (int j = 0; j < output_size.width; j += thumbnail_size.width) {
                    Rect roi(Point(j, i), thumbnail_size);
                    Mat symbol = mats[img.at<uchar>(i / thumbnail_size.height, j / thumbnail_size.width) / 4];
                    symbol.copyTo(output_mat(roi));
                }
            }
            printf("進度: %f%%\r", (process++ / super::frame_total) * 100);
            super::type == IMG ? (void)imwrite("output_pic.png", output_mat) : super::writer.write(output_mat);
        });
    }

    void braille() {
        //垂直限制能被4整除，水平限制2整除
        SettingDataPack pack = SettingDataPack::create(param, "collage_output")
                            .set_color(COLOR_BGR2GRAY)
                            .set_dsize("braille");
        int process = 0;
        bool auto_thresh = false;

        Size output_size = { pack.dsize.width * 4, pack.dsize.height * 4};
        Mat output_mat(output_size.height, output_size.width, CV_8UC3);
        vector<vector<char>> braille_string(pack.dsize.width, vector<char>(pack.dsize.height));

        if (pack.thresh == -1) {
            auto_thresh = true;
        }

        if (super::type == IMG)
            pack.dsize = Size(super::img.cols / 4, super::img.rows / 4);
        else
            super::create_written(pack.dsize, output_size);

        auto mats = read_img("font\\braille\\");
        Size thumbnail_size = {mats.begin()->second.cols, mats.begin()->second.rows};

        cout << output_size.width << ", " << output_size.height << "\n";
        cout << braille_string.size() << ", " << pack.dsize.height << "\n";

        super::basic_handle(pack, [&]() {
            if (auto_thresh) {
                pack.thresh = mean(super::img).val[0];
            }

            braille_create(braille_string, pack.thresh, pack.dsize);

            for (int i = 0, row = 3; i < output_size.height; i += thumbnail_size.height, row += 4) {
                for (int j = 0, col = 0; j < output_size.width; j += thumbnail_size.width, col++) {
                    string buf = {
                            braille_string[row - 3][col], braille_string[row - 2][col], braille_string[row - 1][col],
                            braille_string[row][col]
                    };
                    Rect roi(Point(j, i), thumbnail_size);
                    Mat symbol = mats[buf + ".png"];
                    symbol.copyTo(output_mat(roi));
                }
            }
            super::type == IMG ? (void)imwrite("output_pic.png", output_mat) : super::writer.write(output_mat);
            printf("進度: %f%%\r", (process++ / super::frame_total) * 100);
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
    console_shows.braille();
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
