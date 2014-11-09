#include "MaskCopy.h"

namespace cvImagePipeline {
	namespace Filter {
		using namespace std;
		using namespace cv;
		IMPLEMENT_CVFILTER(MaskCopy);
		MaskCopy::MaskCopy() {
			setInputMatDesc("", "source");
			defInputMat("mask", "mask;  output=(source & mask)");
		}
		MaskCopy::~MaskCopy() { }
		void MaskCopy::execute() {
			const Mat& input_image = getInputMat();
			const Mat& mask = getInputMat("mask");
			Mat& output = refOutputMat();
			output = cv::Mat::zeros(input_image.rows, input_image.cols, input_image.type());
			input_image.copyTo(output, mask);
		}
	}
}
