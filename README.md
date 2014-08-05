# cvImagePipeline - OpenCV画像処理フィルタシーケンスライブラリ。

## 概要

単純な画像処理ルーチンを組み合わせ、一連の画像処理を構成できるライブラリです。

__画像処理プロセッサ__
このライブラリでは、OpenCVに含まれる画像処理ルーチンを、その入出力イメージ(`cv::Mat`)やパラメータとともに、このライブラリで「[画像処理プロセッサ](#ImageProcessor)」と呼ぶクラスに実装し、
その出力を、他の入力へ接続することで、一連の画像処理を構成できます。

__拡張可能__  
いくつかの汎用的な（単純な）画像処理プロセッサは[実装済み](#processor)ですが、足りないものはユーザー独自に作成可能です。

__階層化__  
一連の画像処理もまた、画像処理プロセッサであるため、画像処理の階層化も簡単に可能。

__XMLファイルからの入力__  
単純なプロセッサの接続は[XMLファイル例](https://github.com/takamin/cvImageBlock/blob/master/sample/sample.xml)に記述可能です。
XMLの読み込み関しては[サンプルプログラム](https://github.com/takamin/cvImageBlock/blob/master/sample/capture.cpp)も参照してください。

__動的な組み換え__  
画像処理プロセッサ間の接続は、実行時にも変更可能です。


### 開発環境

* 出力ターゲット Win32 DLL
* 統合開発環境 VisualStudio Express 2013 for Windows Desktop
* プラットフォーム Win32(x86)
* OpenCV 2.4.8とリンクします。[DOWNLOADS|OpenCV](http://opencv.org/downloads.html)からダウンロードして`C:\opencv`に展開（`C:\opencv\build`ディレクトリがある状態）。
* 実行時には c:\opencv\build\x86\vc12\bin にPATHを通す必要があります。
* xmlパーサーとして、[pugixml-1.4](http://pugixml.org/)を利用しています。
* [capture.cpp](https://github.com/takamin/cvImageBlock/blob/master/sample/capture.cpp).


### ビルド方法

ビルドは [src/cvImagePipeline](https://github.com/takamin/cvImageBlock/blob/master/src)のソリューションで行います。

CMake用に、CMakeLists.txtを記述していますが、正常動作不明です。

## 詳細説明

### <a name="ImageProcessor">画像処理プロセッサ</a>

画像処理プロセッサは[抽象クラス`ImageProcessor`]((https://github.com/takamin/cvImageBlock/blob/master/include/ImageProcessor.h))を継承して記述されます。

* ひとつの出力画像(`const cv::Mat&`)を提供します。
* 複数の入力画像(`const cv::Mat&`)を参照します。
* 複数のプロパティを保持できます。処理のパラメータとして使用できます。
* 出力画像は複数のプロセッサから参照可能です。
* 入力画像とプロパティには名前によるアクセスが可能。

#### クラス名による実行時動的インスタンス生成

クラス宣言でDECLARE_CVFILTER、定義時にIMPLEMENT_CVFILTER マクロを使用すれば、クラスの名前でインスタンスを生成できます。
動的生成に対応していない場合は、後述のXMLファイルからの構築はできません。


### <a name="processors">実装済み基本プロセッサ</a>

以下のプロセッサが実装されています。

|クラス				|概要															|
|:---				|:--															|
| VideoCapture		| カメラまたは動画ファイルのキャプチャ(==`cv::VideoCapture`)	|
| ImageWindow		| 画面表示(== `imshow`)	|
| Convert			| フォーマット変換(== `cv::Mat::convertTo`)	| 
| ColorConverter	| チャンネル数変換(== `cv::cvtColor`)	|
| EqualizeHist		| ヒストグラム平坦化(== `cv::equalizeHist`)	|
| GaussianBlur		| ガウシアン平滑化(== `cv::GaussianBlur`)	|
| Flipper			| 反転。(== `cv::flip`)	|
| Resizer			| リサイズ(== `cv::resize`)	|
| BackgroundSubtractor	| MOG/MOG2/GMG 背景除去	|
| RunningAvg		| 長期平均(== `cv::runningAvg`)	|
| AbsDiff			| 絶対値差分(== `cv::absdiff`)	|
| SubMat			| 差分(== `cv::sub`)	|
| Threshold			| 2値化(== `cv::threshold`)	|
| Dilate			| 画像の膨張(== `cv::dilate`)	|
| Erode				| 画像の縮小(== `cv::erode`)	|
| MaskCopy			| マスク処理(== `cv::copyTo`)	|
| ImagePoint		| 無処理(== `cv::copyTo`)。入力画像をそのまま出力します。画像の取り出しポイントとして利用可能。	|
| FitInGrid			| 複数の画像をパネル状に並べて1枚にまとめるプロセッサ。	|
| ImgProcSet		| 任意の画像処理プロセッサで構成可能な汎用の画像処理プロセッサ。	|

### 画像処理プロセッサセット

任意の複数の画像処理プロセッサを保持し、順次処理を行うプロセッサです。
⇒[`ImgProcSet`クラス]((https://github.com/takamin/cvImageBlock/blob/master/include/ImgProcSet.h))です。

内部に保持するプロセッサ間の接続を切り替え可能。

* 複数の画像処理プロセッサを保持します。
* XMLファイルを読み込んで、構成できます。
* 内部のプロセッサの接続がどのようになっていても、画像処理は追加された順序で実行されます。

