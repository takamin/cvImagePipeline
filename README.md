cvImagePipeline
===============

## 概要

OpenCV画像処理フィルタシーケンスライブラリ。

既存の複数の画像処理プロセッサをつなぎ合わせて、ひとつの画像処理プロセッサセットを形成し、一連の画像処理をまとめて実行できるライブラリです（⇒[サンプル](https://github.com/takamin/cvImageBlock/blob/master/sample/capture.cpp)）。
これはXMLファイルからも生成可能です（⇒[XMLファイル例](https://github.com/takamin/cvImageBlock/blob/master/test/certain.xml)）。

汎用的な（単純な）画像処理プロセッサは実装済みです。また、新たな画像処理プロセッサクラスも作成可能。

### 開発環境

* 出力ターゲット Win32 DLL
* 統合開発環境 VisualStudio 2010 Express C++
* プラットフォーム Win32(x86)
* OpenCV 2.4.8とリンクします。[DOWNLOADS|OpenCV](http://opencv.org/downloads.html)からダウンロードして`C:\opencv`に展開（`C:\opencv\build`ディレクトリがある状態）。

### ビルド方法

ビルドは [src/cvImagePipeline](https://github.com/takamin/cvImageBlock/blob/master/src)のソリューションで行います。

CMakeLists.txtを記述していますが、正常動作不明です。

## 詳細説明

### 画像処理プロセッサ

画像処理プロセッサは[抽象クラス`ImageProcessor`]((https://github.com/takamin/cvImageBlock/blob/master/include/ImageProcessor.h))を継承して記述されます。

* ひとつの出力画像を提供します。
* 複数の入力画像を参照します。
* 複数のプロパティを保持できます。処理のパラメータとして使用できます。
* 出力画像は複数のプロセッサから参照可能です。
* 入力画像とプロパティには名前によるアクセスが可能。

#### クラス名による実行時動的インスタンス生成

クラス宣言でDECLARE_CVFILTER、定義時にIMPLEMENT_CVFILTER マクロを使用すれば、クラスの名前でインスタンスを生成できます。
動的生成に対応していない場合は、XMLファイルからの構築はできません。

### 画像処理プロセッサセット

画像処理プロセッサは[`ImgProcSet`クラス]((https://github.com/takamin/cvImageBlock/blob/master/include/ImgProcSet.h))です。

* 複数の画像処理プロセッサを保持します。
* XMLファイルを読み込んで、内部プロセッサを構成し、接続を行えます。
* 内部のプロセッサの接続がどのようになっていても、画像処理は追加された順序で実行されます。

## description

Image processing filter sequence library with OpenCV.

All image processings are executed by image processor.

### build environment

* OpenCV 2.4.8 on C:\\opencv
* Visual Studio 2010 VC++

## class ImageProcessor

The image processor is a instance of a class that is
derived from ImageProcessor.
ImageProcessor provide output image as cv::Mat.
it can connect to other processor's input.
And the class may have input images and
some properties that is accessable with its name.

## create processor by name

A image processor is able to create by its class name.
And its properties be able to accessed by its name.

## create processor by xml

creates processor set from xml by ImgProcSet::loadXml(const string& filename).

see the sample xml [certain.xml](https://github.com/takamin/cvImageBlock/blob/master/test/certain.xml).

## sample code

see the sample program [capture.cpp](https://github.com/takamin/cvImageBlock/blob/master/sample/capture.cpp).


----

This library is using a xml parser [pugixml-1.4](http://pugixml.org/).

