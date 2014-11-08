#pragma once
#include <string>
#include <opencv2/opencv.hpp>

namespace cvImagePipeline {
	SHARED 
	int cvWaitKeyEx(int delay = 1);

	SHARED
	std::string summary(const cv::Mat& image);

	SHARED
	IplImage* createVideoAverageImage(CvCapture* capture);
}
