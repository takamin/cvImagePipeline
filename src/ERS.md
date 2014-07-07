External Reference Specification
================================

## ImageProcessor classes in namespace `cvImagePipeline::Filter`

* AbsDiff
* AverageMat
* AverageOnline
* ColorConverter
* Convert
* DepthTo32F
* Dilate
* EqualizeHist
* Erode
* FeatureTracker
* Flipper
* FlowTracker
* GaussianBlur
* ImagePoint
* ImageWindow
* ImgProcSet
* Labeler
* OpticalFlowFarneback
* Resizer
* Threshold
* VideoCapture

## Properties of Processors in namespace `cvImagePipeline::Filter`


|Class					|Property					|
|:---					|:---						|
|AbsDiff				|(none)						|
|AverageMat				|averageCount				|
|AverageOnline			|averageCount				|
|ColorConverter			|cvtCode					|
|Convert				|rtype, alpha, beta			|
|DepthTo32F				|(none)						|
|Dilate					|iterations					|
|EqualizeHist			|iterations					|
|Erode					|iterations					|
|FeatureTracker			|labeling_region_min_size	|
|Flipper				|flipDir					|
|FlowTracker			|labeling_vec_thrs, labeling_region_min_size	|
|GaussianBlur			|kernelWidth, kernelHeight, sigmaX, sigmaY, borderType	|
|ImagePoint				|(none)						|
|ImageWindow			|windowName, showFrameNumber, showFPS	|
|ImgProcSet				|(none)						|
|Labeler				|labeling_region_min_size	|
|OpticalFlowFarneback	|pyr_scale, levels, winsize, iterations, poly_n, poly_sigma, flags	|
|Resizer				|width, height				|
|Threshold				|thresh, maxval, type		|
|VideoCapture			|deviceIndex, filename, startFrame, stopFrame, width, height	|

## Image Processor Detail

### class AbsDiff

2�̉摜�̍��̐�Βl���o�͂���B
cv::absdiff �Ɠ����̏����B

#### Input

* (�K��̓���) - cv::absdiff�̑������B�������摜
* `"subImage"` - cv::absdiff�̑������B�����摜


### class Convert

�r�b�g�[�x�ƃ`���l������ϊ�����B
cv::Mat::convertTo �Ɠ����B

#### Property

* int rtype - �o�͉摜�̃r�b�g�[�x�ƃ`���l���BCV_8UC1,CV_32FC3 �Ȃǂ�CV_MAKETYPE(depth,channels)�ō쐬�ł���l�B
* double alpha - �r�b�g�[�x�ϊ����ɁA���̉�f�l�ւ�����l�B
* double beta - �r�b�g�[�x�ϊ����Aalpha����������ɑ����l�B

### Dilate

�c���Bcv::dilate�Ɠ��������B

#### Property

* int iterations - �J��Ԃ��񐔁B�K��l��1

### Erode

�k���Bcv::erode�Ɠ��������B

#### Property

* int iterations - �J��Ԃ��񐔁B�K��l��1

### EqualizeHist

�q�X�g�O�����̕�����

### Threshold

��l���Bcv::threshold �Ɠ����B

### VideoCapture

�摜�̓��́B���s����邽�тɁA�J�����A����t�@�C������V�����t���[�������o���ďo�͂���B


### ImageWindow

���O�t���E�B���h�E�ւ̉摜�\���B

#### Property

* std::string windowName - �E�B���h�E���B


