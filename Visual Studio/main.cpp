#include <fstream>
#include "pch.h"
#include "n_ConsoleShow.h"

using namespace std;
using Json = nlohmann::json;

int main() {
    int sw = 0;
    string path = "";
    Json json_file;
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
    ConsoleShows console_shows(path, json_file);
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
