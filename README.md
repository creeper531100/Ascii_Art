![alt text](https://github.com/creeper531100/Ascii_Art/blob/master/%E4%BB%8B%E7%B4%B9/title.png?raw=true)
# AsciiArt
一個AsciiArt生成器，本專案使用C++與opencv框架編輯而成的<br>
- AsciiArt影片製作<br>
- BrailleArt影片製作<br>
- 基本影像處理<br>

A AsciiArt generator<br>
- AsciiArt video generator
- BrailleArt video generator
- Basic image processing

# 教學 How to use it?
![alt text](https://github.com/creeper531100/Ascii_Art/blob/master/%E4%BB%8B%E7%B4%B9/main.png?raw=true)<br>
## 作業
  1. 下載完執行 AsciiArt.exe，可以看見共有12項功能，下面單元會講解使用方法<br>
  2. 選擇完樣式，程式會要求你導入影像，這時候你可以
    * 輸入一般影像/影片(jpg, png, mp4)等檔案
    * 輸入Youtube網址來自動下載影片，只要貼入網址就好了，下載完的影片為`HTC.mp4`
  3. 等待輸出影片/影像... 過成依影片長度決定
  4. 

# 功能介紹 Features
* AsciiArt
  * 預覽AsciiArt - 他可以立即輸出影片到WindowsConsole，不需等待匯出，但並沒有聲音
  * 輸出AsciiArt - 將匯入的影片輸出成平常的mp4檔，可以分享給你所有的朋友
  * 快速輸出AsciiArt - 使用opencv預設的字體作為輸出，但是解析度較低，字體效果也較差
  * 輸出AsciiArt 彩色版 - 使用opencv預設的字體作為輸出，但是是彩色的
  * 輸出AsciiArt 彩色版(固定文字) - 使用單一字形作為輸出，不會因為亮度改變字形
* BrailleArt
  * 預覽BrailleArt - 此效果可能會卡住，他可以立即輸出影片到WindowsConsole，不需等待匯出，但並沒有聲音
  * 輸出BrailleArt - 將匯入的影片輸出成平常的mp4檔，可以分享給你所有的朋友
* 基本影像處理 
  * 輸出帥臉 - 將影像進行二值化，變成剪影，也給你朋友當作一個大驚喜
  * 輸出浮雕照 - 對影像作分離，可以達成一個非常酷的效果
  * 函數處理 - 對影像做基本的函數處理
  * 素描/描邊 - 對影像做canny處理，可以達成描邊效果
