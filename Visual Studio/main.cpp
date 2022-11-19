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

    auto a = ConsoleShowPack::create(json_file).set_dsize("ascii");
    cout << a.dsize << endl;

    cin >> sw;
    cout << u8"輸入網址或圖片位置: ";

    std::getline(std::cin >> std::ws, path);

    if (path.find("https") != std::string::npos) {
        remove("HTC.mp4");
        ::system(("youtube-dl -o HTC.%(ext)s -f mp4 " + path).c_str());
        path = "HTC.mp4";
    }

    ImageHandle *img_handle[] = {
        new ConsoleShows(path, json_file),
        new CollageOutput(path, json_file),
        new QuickOutput(path, json_file)
    };


    switch (sw) {
    case 0: {
        img_handle[0]->ascii();
        break;
    }
    case 1: {
        img_handle[0]->braille();
        break;
    }
    case 2: {
        img_handle[1]->ascii();
        break;
    }
    case 3: {
        img_handle[1]->braille();
        break;
    }
    case 4: {
        img_handle[1]->qt();
        break;
    }
    case 5: {
        img_handle[2]->ascii(QuickOutput::DEFAULT);
        break;
    }
    case 6: {
        img_handle[2]->ascii(QuickOutput::COLOR);
        break;
    }
    case 7: {
        img_handle[2]->ascii(QuickOutput::FILLED);
        break;
    }
    default:
        cout << u8"很遺憾失敗了" << endl;
    }

    for (int i = 0; i < 3; ++i) {
        delete img_handle[i];
    }

    ::system("pause");
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