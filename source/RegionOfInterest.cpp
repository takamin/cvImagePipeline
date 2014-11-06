#include "RegionOfInterest.h"

namespace cvImagePipeline {
	namespace Filter {
		
		IMPLEMENT_CVFILTER(RegionOfInterest);

		RegionOfInterest::RegionOfInterest()
			: left("left", 0), top("top", 0), width("width", 0), height("height", 0)
		{
			defParam(left);
			defParam(top);
			defParam(width);
			defParam(height);
		}


		RegionOfInterest::~RegionOfInterest()
		{
		}

		void RegionOfInterest::execute() {
			const cv::Mat& inputMat = getInputMat();
			int w = width;
			int h = height;
			int x = left;
			int y = top;
			if (x < 0) {
				w += x;
				x = 0;
			}
			if (x + w > inputMat.cols) {
				w -= (x + w) - inputMat.cols;
			}
			if (y < 0) {
				h += y;
				y = 0;
			}
			if (y + h > inputMat.rows) {
				h -= (y + h) - inputMat.rows;
			}
			if (w <= 0 || h <= 0) {
				setOutputMat(cv::Mat());
				return;
			}
			setOutputMat(inputMat(cv::Rect(x, y, w, h)));
		}
	}
}
