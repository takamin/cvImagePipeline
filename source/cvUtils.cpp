#if defined(_MSC_VER)
#include <windows.h>
#endif
#include <stdio.h>
#include <sstream>
#include <opencv2/opencv.hpp>
#if defined(_MSC_VER)
#include <opencv2/nonfree/nonfree.hpp>
#endif
#include "ImageProcessor.h"

#if defined(_MSC_VER)
BOOL WINAPI DllMain( HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved)
{
	cv::initModule_nonfree(); 
	return TRUE;
}
#endif

namespace cvImagePipeline {
	SHARED
	int cvWaitKeyEx(int delay /* =1 */) {
		int ch = cvWaitKey(delay);
		if (ch == '\x1b') {
			fprintf(stderr, "push [ESC] to exit, or continue.");
			while((ch = cvWaitKey(100)) == -1) {/**/}
		}
		return ch;
	}
	SHARED
	std::string summary(const cv::Mat& image) {
		using namespace std;
		stringstream ss;
		ss << "width:" << image.cols << ",";
		ss << "height:" << image.rows << ",";
		ss << "channels:" << image.channels() << ",";
		ss << "depth:";
		int depth = image.depth();
		switch (depth) {
		case CV_8S: ss << "CV_8S"; break;
		case CV_8U: ss << "CV_8U"; break;
		case CV_16S: ss << "CV_16S"; break;
		case CV_16U: ss << "CV_16U"; break;
		case CV_32F: ss << "CV_32F"; break;
		case CV_32S: ss << "CV_32S"; break;
		case CV_64F: ss << "CV_64F"; break;
		default: ss << "?"; break;
		}
		ss << "(" << depth << "),";
		ss << std::endl;
		return ss.str();
	}
}
