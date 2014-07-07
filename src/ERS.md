External Reference Specification
================================

## Image Processors

### class AbsDiff

2つの画像の差の絶対値を出力する。
cv::absdiff と同等の処理。

#### Input

* (規定の入力) - cv::absdiffの第一引数。引かれる画像
* `"subImage"` - cv::absdiffの第二引数。引く画像


### class Convert

ビット深度とチャネル数を変換する。
cv::Mat::convertTo と同等。

#### Property

* int rtype - 出力画像のビット深度とチャネル。CV_8UC1,CV_32FC3 などのCV_MAKETYPE(depth,channels)で作成できる値。
* double alpha - ビット深度変換時に、元の画素値へかける値。
* double beta - ビット深度変換時、alphaをかけた後に足す値。

### Dilate

膨張。cv::dilateと同じ処理。

#### Property

* int iterations - 繰り返し回数。規定値は1

### Erode

縮小。cv::erodeと同じ処理。

#### Property

* int iterations - 繰り返し回数。規定値は1

### EqualizeHist

ヒストグラムの平滑化

### Threshold

二値化。cv::threshold と同じ。

### VideoCapture

画像の入力。実行されるたびに、カメラ、動画ファイルから新しいフレームを取り出して出力する。


### ImageWindow

名前付きウィンドウへの画像表示。

#### Property

* std::string windowName - ウィンドウ名。

