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
		type("type", ThreshBinary),
		otsu("otsu", false)
		{
			defParam(thresh);
			defParam(maxval);
			defParam(type);
			defParam(otsu);

			updateThreashType();
		}
		Threshold::~Threshold() { }
		void Threshold::execute() {
			const Mat& input_image = getInputMat();
			if(input_image.empty()) {
				return;
			}
			Mat& output = refOutputMat();
			cv::threshold(input_image, output, thresh, maxval, thresh_type);
		}
		void Threshold::onPropertyChange(Property& property) {
			const string& name = property.getName();
			if (name == "type" || name == "otsu") {
				updateThreashType();
			}
		}
		void Threshold::updateThreashType() {
			thresh_type = ((type & CV_THRESH_MASK) | (otsu ? CV_THRESH_OTSU : 0));
		}
	}
}
