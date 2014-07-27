cvImagePipeline
===============

## �T�v

OpenCV�摜�����t�B���^�V�[�P���X���C�u�����B

�����̕����̉摜�����v���Z�b�T���Ȃ����킹�āA�ЂƂ̉摜�����v���Z�b�T�Z�b�g���`�����A��A�̉摜�������܂Ƃ߂Ď��s�ł��郉�C�u�����ł��i��[�T���v��](https://github.com/takamin/cvImageBlock/blob/master/sample/capture.cpp)�j�B
�����XML�t�@�C������������\�ł��i��[XML�t�@�C����](https://github.com/takamin/cvImageBlock/blob/master/test/certain.xml)�j�B

�ėp�I�ȁi�P���ȁj�摜�����v���Z�b�T�͎����ς݂ł��B�܂��A�V���ȉ摜�����v���Z�b�T�N���X���쐬�\�B

### �J����

* �o�̓^�[�Q�b�g Win32 DLL
* �����J���� VisualStudio Express 2013 for Windows Desktop
* �v���b�g�t�H�[�� Win32(x86)
* OpenCV 2.4.8�ƃ����N���܂��B[DOWNLOADS|OpenCV](http://opencv.org/downloads.html)����_�E�����[�h����`C:\opencv`�ɓW�J�i`C:\opencv\build`�f�B���N�g���������ԁj�B
* ���s���ɂ� c:\opencv\build\x86\vc12\bin ��PATH��ʂ��K�v������܂��B

### �r���h���@

�r���h�� [src/cvImagePipeline](https://github.com/takamin/cvImageBlock/blob/master/src)�̃\�����[�V�����ōs���܂��B

CMakeLists.txt���L�q���Ă��܂����A���퓮��s���ł��B

## �ڍא���

### �摜�����v���Z�b�T

�摜�����v���Z�b�T��[���ۃN���X`ImageProcessor`]((https://github.com/takamin/cvImageBlock/blob/master/include/ImageProcessor.h))���p�����ċL�q����܂��B

* �ЂƂ̏o�͉摜(`const cv::Mat&`)��񋟂��܂��B
* �����̓��͉摜(`const cv::Mat&`)���Q�Ƃ��܂��B
* �����̃v���p�e�B��ێ��ł��܂��B�����̃p�����[�^�Ƃ��Ďg�p�ł��܂��B
* �o�͉摜�͕����̃v���Z�b�T����Q�Ɖ\�ł��B
* ���͉摜�ƃv���p�e�B�ɂ͖��O�ɂ��A�N�Z�X���\�B

#### �N���X���ɂ����s�����I�C���X�^���X����

�N���X�錾��DECLARE_CVFILTER�A��`����IMPLEMENT_CVFILTER �}�N�����g�p����΁A�N���X�̖��O�ŃC���X�^���X�𐶐��ł��܂��B
���I�����ɑΉ����Ă��Ȃ��ꍇ�́A��q��XML�t�@�C������̍\�z�͂ł��܂���B


### �����ς݊�{�v���Z�b�T

�ȉ��̃v���Z�b�T����������Ă��܂��B

|�N���X				|�T�v															|
|:---				|:--															|
| VideoCapture		| �J�����܂��͓���t�@�C���̃L���v�`��(==`cv::VideoCapture`)	|
| ImageWindow		| ��ʕ\��(== `imshow`)	|
| Convert			| �t�H�[�}�b�g�ϊ�(== `cv::Mat::convertTo`)	| 
| ColorConverter	| �`�����l�����ϊ�(== `cv::cvtColor`)	|
| EqualizeHist		| �q�X�g�O�������R��(== `cv::equalizeHist`)	|
| GaussianBlur		| �K�E�V�A��������(== `cv::GaussianBlur`)	|
| Flipper			| ���]�B(== `cv::flip`)	|
| Resizer			| ���T�C�Y(== `cv::resize`)	|
| RunningAvg		| ��������(== `cv::runningAvg`)	|
| AbsDiff			| ��Βl����(== `cv::absdiff`)	|
| SubMat			| ����(== `cv::sub`)	|
| Threshold			| 2�l��(== `cv::threshold`)	|
| Dilate			| �摜�̖c��(== `cv::dilate`)	|
| Erode				| �摜�̏k��(== `cv::erode`)	|
| MaskCopy			| �}�X�N����(== `cv::copyTo`)	|
| ImagePoint		| ������(== `cv::copyTo`)�B���͉摜�����̂܂܏o�͂��܂��B�摜�̎��o���|�C���g�Ƃ��ė��p�\�B	|
| FitInGrid			| �����̉摜��1���ɂ܂Ƃ߂�B	|
| ImgProcSet		| �C�ӂ̉摜�����v���Z�b�T�ō\���\�Ȕėp�̉摜�����v���Z�b�T�B	|
| ImgProcThread		| ImgProcSet�̏������T�u�X���b�h�Ŏ��s����v���Z�b�T�i�����������j	|

### �摜�����v���Z�b�T�Z�b�g

�C�ӂ̕����̉摜�����v���Z�b�T��ێ����A�����������s���v���Z�b�T�ł��B
��[`ImgProcSet`�N���X]((https://github.com/takamin/cvImageBlock/blob/master/include/ImgProcSet.h))�ł��B

�����ɕێ�����v���Z�b�T�Ԃ̐ڑ���؂�ւ��\�B

* �����̉摜�����v���Z�b�T��ێ����܂��B
* XML�t�@�C����ǂݍ���ŁA�\���ł��܂��B
* �����̃v���Z�b�T�̐ڑ����ǂ̂悤�ɂȂ��Ă��Ă��A�摜�����͒ǉ����ꂽ�����Ŏ��s����܂��B


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

