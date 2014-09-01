#pragma once
#include <string>
#include <opencv2/opencv.hpp>

namespace cvImagePipeline {
	__declspec(dllexport) 
	int cvWaitKeyEx(int delay = 1);

	__declspec(dllexport)
	std::string summary(const cv::Mat& image);

	__declspec(dllexport)
	IplImage* createVideoAverageImage(CvCapture* capture);
}
