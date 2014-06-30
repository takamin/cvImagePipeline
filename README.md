cvImageBlock
============

## description

Image processing pipeline library with OpenCV for Windows.

All image processings are executed by image processor block.

* OpenCV 2.4.8 on C:\\opencv
* Visual Studio 2010 VC++

## image processor block isa class ImageProcessor

The image processor block is a instance of a class that is
derived from ImageProcessor.
it has input and output image, and it may have some processing properties.

The output image can connect to other processor's input.

## creation

A image processor is able to create by its class name.
And its properties be able to accessed by its name.

## sample code

see the sample program [capture](https://github.com/takamin/cvImageBlock/blob/master/src/capture/capture.cpp)
