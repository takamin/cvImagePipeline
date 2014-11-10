# cvImagePipeline - OpenCV画像処理フィルタシーケンスライブラリ。

## 概要

[cvImagePipeline](https://github.com/takamin/cvImagePipeline)
は、OpenCVを利用した単純な__画像処理プロセッサ__を組み合わせて、一連の画像処理を構成するためのライブラリです。

__入出力の自由な接続__  
画像処理プロセッサの出力を他のプロセッサの入力へ接続できます。
この接続は、実行時にも変更可能です。

__拡張可能__  
いくつかの汎用的で単純な画像処理プロセッサは[実装済み](#processor)ですが、
新たなプロセッサが必要になれば、ImageProcessorクラスから継承して独自に画像処理プロセッサを作成できます。

__画像処理の階層化・モジュール化__  
接続された複数の画像処理プロセッサをまとめて、ひとつの画像処理プロセッサとみなすことが出来ます。

__XMLファイルからの入力__  
プロセッサの接続は[XMLファイル](https://github.com/takamin/cvImageBlock/blob/master/sample/sample.xml)に記述できます。
詳細は[サンプルプログラム](#sample)を参照してください。

### ビルド方法

cmakeを使用してください。Windows 7(32bit)Visual Studio 2010、Ubuntu 14.04 LTS で動作を確認しています。
OpenCV 2.4が必要です。2.4.8/2.4.9/2.4.10で動作を確認しています。[DOWNLOADS|OpenCV](http://opencv.org/downloads.html)からダウンロードして`C:\opencv`に展開（`C:\opencv\build`ディレクトリがある状態）。

```
mkdir build
cd build
cmake -D OpenCV_DIR='path/to/opencv' ..
```

### <a name="sample"></a>サンプルプログラム

[XMLファイル](https://github.com/takamin/cvImageBlock/blob/master/sample/sample.xml)
を読み込んでカメラ画像を表示する
[サンプルプログラム](https://github.com/takamin/cvImageBlock/blob/master/sample/capture.cpp)です。

`$ sample `[`sample.xml`](https://github.com/takamin/cvImageBlock/blob/master/sample/sample.xml)


## 詳細説明

### <a name="processors"></a>基本プロセッサ

以下のプロセッサが基本プロセッサです。

|クラス				|概要															|
|:---				|:--															|
| ImageProcessor    | 画像処理プロセッサの基本クラスです。複数の入力画像、ひとつの出力画像、複数の型付けされたプロパティを提供します。                          |
| VideoCapture		| カメラまたは動画ファイルのキャプチャ(==`cv::VideoCapture`)	|
| ImageWindow		| 画面表示(== `imshow`)	|
| FitInGrid			| 複数の画像をパネル状に並べて1枚にまとめるプロセッサ。	|
| ImgProcSet		| 任意の画像処理プロセッサで構成可能な汎用の画像処理プロセッサ。	|
| ImagePoint		| 無処理(== `cv::copyTo`)。入力画像をそのまま出力します。画像の取り出しポイントとして利用可能。	|

### その他のプロセッサ。

以下のクラスは、サンプル的に実装した単純な画像処理を行うプロセッサです。新たなプロセッサを作成するときなどにも参考にしてください。

|クラス				|概要															|
|:---				|:--															|
| Convert			| フォーマット変換(== `cv::Mat::convertTo`)	| 
| ColorConverter	| チャンネル数変換(== `cv::cvtColor`)	|
| EqualizeHist		| ヒストグラム平坦化(== `cv::equalizeHist`)	|
| GaussianBlur		| ガウシアン平滑化(== `cv::GaussianBlur`)	|
| Flipper			| 反転。(== `cv::flip`)	|
| Resizer			| リサイズ(== `cv::resize`)	|
| RunningAvg		| 長期平均(== `cv::runningAvg`)	|
| AbsDiff			| 絶対値差分(== `cv::absdiff`)	|
| SubMat			| 差分(== `cv::sub`)	|
| Threshold			| 2値化(== `cv::threshold`)	|
| Dilate			| 画像の膨張(== `cv::dilate`)	|

### クラス名による実行時動的インスタンス生成について

クラス宣言でDECLARE_CVFILTER、定義時にIMPLEMENT_CVFILTER マクロを使用すれば、クラスの名前でインスタンスを生成できます。
動的生成に対応していない場合は、後述のXMLファイルからの構築はできません。

