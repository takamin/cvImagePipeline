cvImagePipeline
===============

## description

Image processing pipeline library with OpenCV for Windows.

All image processings are executed by image processor.

To build, following items are required.

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

see the sample xml [certain.xml](https://github.com/takamin/cvImageBlock/blob/master/src/test/certain.xml).

## sample code

see the sample program [capture](https://github.com/takamin/cvImageBlock/blob/master/src/capture/capture.cpp).


----

This library is using a xml parser [pugixml-1.4](http://pugixml.org/).

