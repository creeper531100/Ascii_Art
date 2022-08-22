#define _CRT_SECURE_NO_WARNINGS
#include <fstream>

#include "n_CollageOutput.h"
#include "pch.h"
#include "n_ConsoleShow.h"
//#include "BasicEffectOutput.h"
#include "n_QuickOutput.h"

using namespace std;
using Json = nlohmann::json;

int main() {
    SetConsoleOutputCP(65001);
    int sw = 0;
    string path = "";
    Json json_file;
    ifstream in("setting.json");
    in >> json_file;

    cout << u8"AsciiArt新藝術\n";
    cout << u8"----------------------------------\n";
    cout << u8"(0).預覽 AsciiArt(1).預覽 BrailleArt \n";
    cout << u8"(2).輸出 AsciiArt(3).輸出 BrailleArt \n";
    cout << u8"(4).輸出四元樹圖像\n";
    cout << u8"(5).快速輸出 AsciiArt(6).輸出 AsciiArt 彩色版(7).輸出 AsciiArt 彩色版(固定文字)\n";
    cout << u8"(x).輸出浮雕照 (x).函數處理(x).素描(x).描邊 (x).帥臉\n";
    cout << u8"----------------------------------\n選擇功能: ";

    cin >> sw;
    cout << u8"輸入網址或圖片位置: ";
    std::getline(std::cin >> std::ws, path);
    if (path.find("https") != std::string::npos) {
        remove("HTC.mp4");
        ::system(("youtube-dl -o HTC.%(ext)s -f mp4 " + path).c_str());
        path = "HTC.mp4";
    }

    switch (sw) {
    case 0: {
        ConsoleShows img_handle(path, json_file);
        img_handle.ascii();
        break;
    }
    case 1: {
        ConsoleShows img_handle(path, json_file);
        img_handle.braille();
        break;
    }
    case 2: {
        CollageOutput img_handle(path, json_file);
        img_handle.ascii();
        break;
    }
    case 3: {
        CollageOutput img_handle(path, json_file);
        img_handle.braille();
        break;
    }
    case 4: {
        CollageOutput img_handle(path, json_file);
        img_handle.qt();
        break;
    }
    case 5: {
        QuickOutput img_handle(path, json_file);
        img_handle.ascii(QuickOutput::DEFAULT);
        break;
    }
    case 6: {
        QuickOutput img_handle(path, json_file);
        img_handle.ascii(QuickOutput::COLOR);
        break;
    }
    case 7: {
        QuickOutput img_handle(path, json_file);
        img_handle.ascii(QuickOutput::FILLED);
        break;
    }
    /*case 8: {
        BasicEffectOutput img_handle(path, json_file);
        img_handle.relief();
        break;
    }
    case 9: {
        BasicEffectOutput img_handle(path, json_file);
        img_handle.sin_handle();
        break;
    }
    case 10: {
        BasicEffectOutput img_handle(path, json_file);
        img_handle.sketch();
        break;
    }
    case 11: {
        BasicEffectOutput img_handle(path, json_file);
        img_handle.trace();
        break;
    }
    case 12: {
        BasicEffectOutput img_handle(path, json_file);
        img_handle.thresholding();
    }*/
    default:
        cout << u8"很遺憾失敗了" << endl;
    }
    ::system("pause");
}
