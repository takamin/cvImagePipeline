#include "Threshold.h"

namespace cvImagePipeline {
	namespace Filter {
		using namespace std;
		using namespace cv;

		IMPLEMENT_CVFILTER(Threshold);
		Threshold::Threshold()
		:
		thresh("thresh", 0.0),
		maxval("maxval", 0.0),
		type("type", 0)
		{
			defParam(thresh);
			defParam(maxval);
			defParam(type);
		}
		Threshold::~Threshold() { }
		void Threshold::execute() {
			const Mat& input_image = getInputMat();
			if(input_image.empty()) {
				return;
			}
			Mat& output = refOutputMat();
			cv::threshold(input_image, output, thresh, maxval, type);
		}
	}
}
