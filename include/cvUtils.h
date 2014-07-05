#pragma once
#include <opencv2/opencv.hpp>

namespace cvImagePipeline {
	__declspec(dllexport) 
	int cvWaitKeyEx(int delay = 1);

	__declspec(dllexport) 
	IplImage* createVideoAverageImage(CvCapture* capture);
}
