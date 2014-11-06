#include "SuperImpose.h"
#include <typeinfo>
#include <time.h>
using namespace std;
namespace cvImagePipeline {
	namespace Filter {
		///////////////////////////////////////////////////////////////////////////
		// class SuperImpose
		cv::Mat SuperImpose::empty_image;
		IMPLEMENT_CVFILTER(SuperImpose);
		SuperImpose::SuperImpose()
			: left("left", 0), top("top", 0), width("width", 0), height("height", 0)
		{
			defParam(left);
			defParam(top);
			defParam(width);
			defParam(height);
			defInputMat("upperImage");
			setInputMat("upperImage", empty_image);
		}
		void SuperImpose::execute(){
			const cv::Mat& inputMat = getInputMat();
			const cv::Mat& upperImage = getInputMat("upperImage");
			cv::Mat& outputMat = refOutputMat();

			inputMat.copyTo(outputMat);
			if (!upperImage.empty()) {
				cv::Rect roi_rect = cv::Rect(left, top,
					width > 0 ? width : inputMat.cols,
					height > 0 ? height : inputMat.rows);
				if (roi_rect.x < 0) {
					roi_rect.x = 0;
					roi_rect.width -= roi_rect.x;
				}
				if (roi_rect.x + roi_rect.width >= inputMat.cols) {
					roi_rect.width = (inputMat.cols - roi_rect.x);
				}
				if (roi_rect.y < 0) {
					roi_rect.y = 0;
					roi_rect.height -= roi_rect.y;
				}
				if (roi_rect.y + roi_rect.height >= inputMat.rows) {
					roi_rect.height = (inputMat.rows - roi_rect.y);
				}
				cv::Mat dst = cv::Mat(outputMat, roi_rect);
				
				cv::Mat overwrap_image;
				cv::resize(upperImage, overwrap_image, cv::Size(dst.cols, dst.rows), 0.0, 0.0, CV_INTER_CUBIC);
				dst *= 0.5;
				dst += overwrap_image * 0.5;
			}
		}
	}
}
