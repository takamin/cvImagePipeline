#include "MaskCopy.h"

namespace cvImagePipeline {
	namespace Filter {
		using namespace std;
		using namespace cv;
		IMPLEMENT_CVFILTER(MaskCopy);
		MaskCopy::MaskCopy() {
			defInputMat("mask");
		}
		MaskCopy::~MaskCopy() { }
		void MaskCopy::execute() {
			const Mat& input_image = getInputMat();
			const Mat& mask = getInputMat("mask");
			if(input_image.empty()) {
				return;
			}
			Mat& output = refOutputMat();
			input_image.copyTo(output, mask);
		}
	}
}
