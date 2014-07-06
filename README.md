cvImagePipeline
===============

## 概要

OpenCV画像処理フィルタシーケンスライブラリ。

既存の複数の画像処理プロセッサをつなぎ合わせて、ひとつの画像処理プロセッサセットを形成し、一連の画像処理をまとめて実行できるライブラリです。
（[サンプル](https://github.com/takamin/cvImageBlock/blob/master/sample/capture.cpp)）
画像処理プロセッサセットは画像処理プロセッサですから、階層的なモデルを形成できます。
これはXMLファイルからも生成可能です。
（[XMLファイル例](https://github.com/takamin/cvImageBlock/blob/master/test/certain.xml)）

汎用的な（単純な）画像処理プロセッサは実装済みです。また、新たな画像処理プロセッサクラスも作成可能。

### 開発環境

* 出力ターゲット Win32 DLL
* 統合開発環境 VisualStudio 2010 Express C++
* プラットフォーム Win32(x86)
* OpenCV 2.4.8とリンクします。[DOWNLOADS|OpenCV](http://opencv.org/downloads.html)からダウンロードして`C:\opencv`にインストールしてください。

### ビルド方法

ビルドは [src/cvImagePipeline](https://github.com/takamin/cvImageBlock/blob/master/src/cvImagePipeline)のソリューションでビルドしてください。

CMakeLists.txtを記述していますが、正常動作不明です。

## 詳細説明

### 画像処理プロセッサ

* ひとつの出力画像
* 複数の入力画像への参照。他のプロセッサの出力画像へ接続
* 複数のプロパティ
* 出力画像は複数のプロセッサから参照可能

### 画像処理プロセッサセット

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

