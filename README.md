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
## 作業 Instructions
  1. 下載完執行 AsciiArt.exe，可以看見共有12項功能，下面單元會講解使用方法<br>
  2. 選擇完樣式，程式會要求你導入影像，這時候你可以<br>
      > 輸入一般影像/影片(jpg, png, mp4)等檔案，拖至視窗內即可，檔案名稱不可以有空白<br>
      > 輸入Youtube網址來自動下載影片，只要貼入網址就好了，下載完的影片為`HTC.mp4`<br>
  3. 等待輸出影片/影像... 過成依影片長度決定
  4. 在資料夾會生成`output_video.mp4`或`output_pic.png`即是輸出檔案
  5. 完成~~~

# 功能介紹 Features
![alt text](https://github.com/creeper531100/Ascii_Art/blob/master/%E4%BB%8B%E7%B4%B9/colordef.png?raw=true)<br>
* AsciiArt
  * `預覽AsciiArt` - 他可以立即輸出影片到WindowsConsole，視窗必須保持最大化且沒有縮放
  * `輸出AsciiArt` - 將匯入的影片輸出成平常的mp4檔，可以分享給你所有的朋友
  * `快速輸出AsciiArt` - 使用opencv預設的字體作為輸出，但是解析度較低，字體效果也較差
  * `輸出AsciiArt 彩色版` - 使用opencv預設的字體作為輸出，但是是彩色的
  * `輸出AsciiArt 彩色版(固定文字)` - 使用單一字形作為輸出，不會因為亮度改變字形
* BrailleArt
  * `預覽BrailleArt` - 此效果可能會卡住，他可以立即輸出影片到WindowsConsole，視窗必須保持最小化且沒有縮放
  * `輸出BrailleArt` - 將匯入的影片輸出成平常的mp4檔，可以分享給你所有的朋友
* 四元樹
  * `輸出四元樹圖像` - 將匯入的影片進行四元樹分割，可以分享給你所有的朋友
* 基本影像處理 `待修復`
  * `輸出帥臉` - 將影像進行二值化，變成剪影，也給你朋友當作一個大驚喜
  * `輸出浮雕照` - 對影像作分離，可以達成一個非常酷的效果
  * `函數處理` - 對影像做基本的函數處理
  * `素描/描邊` - 對影像做canny處理，可以達成描邊效果
# 個人化設定 Setting
若你想對解析度或是二值化域值進行設定，你可以進入`Setting.json`對他設定<br>
```javascript
{
	"console_show": {
		"thresh": -1, //域值設定，當像素大於域值即設定為白色，否則為黑色，-1即為自動
		"ascii": {
			"width": 237, //輸出影片寬設定，-1為自動
			"height": 64  //輸出影片長設定，-1為自動
		},
		"braille": {
			"width": 240,
			"height": 120
		}
	},
	"collage_output": {
		"thresh": -1, //域值設定，-1為自動
		"ascii": {
			"width": 240, //長寬
			"height": 67  //長寬
		},
		"braille": {
			"width": 480, //長寬
			"height": 268 //長寬
		},
		"qt": {
			"texture": "-1", //你的貼文文件位址，-1為自動
			"cap": 7, //要分割多少個小方塊
			"reverse": 0 //啟用反向顏色 0: 關閉
		}
	},
	"basic_effect": {
		"contours_handle": { //輪廓設定
			"thresh": {
				"min": 60, //輪廓最小域質
				"max": 120 //輪廓最大域質
			},
			"color": [0, 0, 0], //輪廓顏色
			"canvas_color": [255, 255, 255] //畫布顏色
		},
		"thresholding": { //二值化設定
			"thresh": -1,
			"width": -1,
			"height": -1
		},
		"relief": { //浮雕照設定
			"effect": 1, //效果
			"offset": 128, //偏移
			"width": -1, //長寬
			"height": -1 //長寬
		},
		"math_function_handle": {
			"deltaI": 10,
			"arg": 5
		}
	},
	"quick_output": {
		"font_size": 5, //拼貼的文字大小
		"fill_char": "@", //填滿文字的字形
		"ascii": {
			"width": 160,
			"height": 71
		}
	}
}
```
# 參考影片
1. [AsciiArt](https://youtu.be/8WSQcTy1UYM)
2. [AsciiArt - QuickOutput](https://youtu.be/o5iKn05nuLc)
3. [BrailleArt](https://youtu.be/fds8-M9iK8I)
4. [QuadTree](https://youtu.be/n1y7JL6NfS8)
# 下載
請點擊左邊Releases
