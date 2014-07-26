#include "stdafx.h"
#include <windows.h>
#include <opencv2/opencv.hpp>
#include "cvImagePipeline.h"

using namespace cvImagePipeline;
using namespace cvImagePipeline::Filter;

//#define FEATURE_TRACKER
#define FLOW_TRACKER

template<class T> void operator >> (const std::string& src, T& dst) {
	std::stringstream(src) >> dst;
}
template<class T> T parse(const std::string& src) {
	T dst;
	std::stringstream(src) >> dst;
	return dst;
}
template<class T> std::string toString(const T& src) {
	std::stringstream ss;
	ss << src;
	return ss.str();
}

int _tmain(int argc, _TCHAR* argv[])
{
	cvInitSystem(argc, argv);

	/////////////////////////////////////////////////////////////////////////
	// root image processing block
	// this includes all the processor blocks.
	/////////////////////////////////////////////////////////////////////////

	ImgProcSet processors;

	/////////////////////////////////////////////////////////////////////////
	// input block
	/////////////////////////////////////////////////////////////////////////

	VideoCapture cap;
	processors.add(cap);

	// argv[1] - video filename or capture device number
	std::string deviceName("#0");
	if(argc > 1) {
		deviceName = argv[1];
	}
	bool open = false;
	if(deviceName.at(0) != '#') {
		// open video file
		open = cap.open(deviceName);
	} else {
		// open capture device
		open = cap.open(parse<int>(deviceName.substr(1)));
	}
	if(!open) {
		std::cerr << deviceName << "fail to open." << std::endl;
		exit(-1);
	}

	/////////////////////////////////////////////////////////////////////////
	// preprocessor block
	/////////////////////////////////////////////////////////////////////////

	ImgProcSet preprocessors;
	processors.add(preprocessors);

	if(deviceName.at(0) == '#') {
		// flip when capturing camera. assumes it selfee.
		preprocessors.add("Flipper", "mirror")
			.property("flipDir", 1);
	}
	preprocessors.add("ImagePoint", "capturedImage");
	preprocessors.add("ColorConverter", "grayScale")
		.property("cvtCode", CV_BGR2GRAY);
	preprocessors.add("EqualizeHist", "equalizeHist");
	
	/////////////////////////////////////////////////////////////////////////
	//optical flow block
	/////////////////////////////////////////////////////////////////////////

	ImgProcSet flowProcessors;
	processors.add(flowProcessors);

	//resize image to small for speed.
	flowProcessors.add("Resizer", "resizerForProc")
		.property("width", 160)
		.property("height", 120);

	//Optical flow
	flowProcessors.add("OpticalFlowFarneback", "optflow")
		.property("winsize", 50)
		.property("pyr_scale", (float)0.5)
		.property("levels", 1)
		.property("iterations", 1);

	//restore flow size to show
	flowProcessors.add("Resizer", "flowResizer")
		.property("width", 640)
		.property("height", 480);

#ifdef FEATURE_TRACKER
	processors.add("FeatureTracker", "featureTracker", false);
	preprocessors >> processors["featureTracker"];
	preprocessors["capturedImage"] >> processors["featureTracker"].input("drawbase");
	ImageWindow featureTrackerImage("FeatureTracker Image");
	featureTrackerImage.showFPS = true;
	featureTrackerImage.showFrameNumber = true;
	processors.add(featureTrackerImage, false);
	processors["featureTracker"] >> featureTrackerImage;
#endif

#ifdef FLOW_TRACKER
	processors.add("FlowTracker", "flowTracker", false)
		.property("labeling_vec_thrs", (float)0.3)
		.property("labeling_region_min_size", 1000);
	flowProcessors >> processors["flowTracker"];
	preprocessors["capturedImage"] >> processors["flowTracker"].input("drawbase");
	ImageWindow flowTrackerImage("FlowTracker Image");
	flowTrackerImage.showFPS = true;
	flowTrackerImage.showFrameNumber = true;
	processors.add(flowTrackerImage, false);
	processors["flowTracker"] >> flowTrackerImage;
#endif

	if(argc > 2) { cap.property("startFrame", argv[2]); }
	if(argc > 3) { cap.property("stopFrame", argv[3]); }

	//prevent to fail with some slow machine or camera.
	Sleep(2000);

	while(true) {

		//all processor blocks
		processors.execute();

		//check [esc] key
		int ch = cvWaitKeyEx(1);
		if (ch == '\x1b') {
			fprintf(stderr, "exit.\n");
			break;
		}
	}
	cvDestroyAllWindows();
	return 0;
}
