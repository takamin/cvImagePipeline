#include "ImageWindow.h"
#include <time.h>
using namespace std;
namespace cvImagePipeline {
	namespace Filter {
		///////////////////////////////////////////////////////////////////////////
		// class ImageWindow

		IMPLEMENT_CVFILTER(ImageWindow);
		ImageWindow::ImageWindow()
			: windowName("windowName", string("cv::Mat")),
			showFrameNumber("showFrameNumber", false), showFPS("showFPS", false),
			frameNumber(0), tick(0), windowCreated(false)
		{
			defParam(windowName);
			defParam(showFrameNumber);
			defParam(showFPS);
		}
		ImageWindow::ImageWindow(std::string windowName)
			: windowName("windowName", windowName),
			showFrameNumber("showFrameNumber", false), showFPS("showFPS", false),
			frameNumber(0), tick(0), windowCreated(true)
		{
			cvNamedWindow(windowName.c_str(), CV_WINDOW_AUTOSIZE);
			defParam(showFrameNumber);
			defParam(showFPS);
		}
		void ImageWindow::execute() {
			if(!windowCreated) {
				cvNamedWindow(((string)windowName).c_str(), CV_WINDOW_AUTOSIZE);
				windowCreated = true;
			}
			const cv::Mat& image = getInputMat();
			long curr_tick = clock();
			if(!image.empty()) {
				cv::Mat im2show = image.clone();
				if(showFrameNumber || showFPS) {
					std::stringstream ss;
					if(showFrameNumber) {
						ss << "[#" << frameNumber << "] ";
					}
					if(showFPS) {
						double fps = (double)CLOCKS_PER_SEC / (curr_tick - tick); 
						ss << std::fixed << fps << "[fps] ";
					}
					cv::putText(im2show, ss.str(), cv::Point(15, 15),
						cv::FONT_HERSHEY_COMPLEX_SMALL, 0.5,
						cv::Scalar(0,255,0), 1); 
				}
				cv::imshow(windowName, im2show);
				setOutputMat(image.clone());
			}
			tick = curr_tick;
			frameNumber++;
		}
	}
}
