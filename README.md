cvImagePipeline
===============

## �T�v

OpenCV�摜�����t�B���^�V�[�P���X���C�u�����B

�����̕����̉摜�����v���Z�b�T���Ȃ����킹�āA�ЂƂ̉摜�����v���Z�b�T�Z�b�g���`�����A��A�̉摜�������܂Ƃ߂Ď��s�ł��郉�C�u�����ł��B
�i[�T���v��](https://github.com/takamin/cvImageBlock/blob/master/sample/capture.cpp)�j
�摜�����v���Z�b�T�Z�b�g�͉摜�����v���Z�b�T�ł�����A�K�w�I�ȃ��f�����`���ł��܂��B
�����XML�t�@�C������������\�ł��B
�i[XML�t�@�C����](https://github.com/takamin/cvImageBlock/blob/master/test/certain.xml)�j

�ėp�I�ȁi�P���ȁj�摜�����v���Z�b�T�͎����ς݂ł��B�܂��A�V���ȉ摜�����v���Z�b�T�N���X���쐬�\�B

### �J����

* �o�̓^�[�Q�b�g Win32 DLL
* �����J���� VisualStudio 2010 Express C++
* �v���b�g�t�H�[�� Win32(x86)
* OpenCV 2.4.8�ƃ����N���܂��B[DOWNLOADS|OpenCV](http://opencv.org/downloads.html)����_�E�����[�h����`C:\opencv`�ɃC���X�g�[�����Ă��������B

### �r���h���@

�r���h�� [src/cvImagePipeline](https://github.com/takamin/cvImageBlock/blob/master/src)�̃\�����[�V�����Ńr���h���Ă��������B

CMakeLists.txt���L�q���Ă��܂����A���퓮��s���ł��B

## �ڍא���

### �摜�����v���Z�b�T

* �ЂƂ̏o�͉摜
* �����̓��͉摜�ւ̎Q�ƁB���̃v���Z�b�T�̏o�͉摜�֐ڑ�
* �����̃v���p�e�B
* �o�͉摜�͕����̃v���Z�b�T����Q�Ɖ\

### �摜�����v���Z�b�T�Z�b�g

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

