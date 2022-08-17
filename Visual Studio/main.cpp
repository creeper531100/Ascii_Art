#include <fstream>

#include "n_CollageOutput.h"
#include "pch.h"
#include "n_ConsoleShow.h"
#include "BasicEffectOutput.h"
#include "n_QuickOutput.h"

using namespace std;
using Json = nlohmann::json;

int main() {
    int sw = 0;
    string path = "";
    Json json_file;
    ifstream in("setting.json");
    in >> json_file;

    cout << "AsciiArt新藝術\n";
    cout << "----------------------------------\n";
    cout << "(0).預覽 AsciiArt(1).預覽 BrailleArt \n";
    cout << "(2).輸出 AsciiArt(3).輸出 BrailleArt \n";
    cout << "(4).輸出qt(5).輸出浮雕照\n";
    cout << "(6).快速輸出 AsciiArt(7).輸出 AsciiArt 彩色版(8).輸出 AsciiArt 彩色版(固定文字)\n";
    cout << "(9).函數處理(10).素描(11).描邊\n";
    cout << "----------------------------------\n選擇功能: ";

    cin >> sw;
    cout << "輸入網址或圖片位置: ";
    cin >> path;
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
    case 4: { //有問題
        /*BasicEffectOutput img_handle(path, json_file);
        img_handle.thresholding();*/
        CollageOutput img_handle(path, json_file);
        img_handle.qt();
        break;
    }
    case 5: {
        BasicEffectOutput img_handle(path, json_file);
        img_handle.relief();
        break;
    }
    case 6: {
        QuickOutput img_handle(path, json_file);
        img_handle.ascii(QuickOutput::DEFAULT);
        break;
    }
    case 7: {
        QuickOutput img_handle(path, json_file);
        img_handle.ascii(QuickOutput::COLOR);
        break;
    }
    case 8: {
        QuickOutput img_handle(path, json_file);
        img_handle.ascii(QuickOutput::FILLED);
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
    default:
        cout << "很遺憾失敗了" << endl;
    }
    ::system("pause");
}
