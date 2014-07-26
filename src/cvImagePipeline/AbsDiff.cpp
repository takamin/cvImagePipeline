#include "AbsDiff.h"

namespace cvImagePipeline {
	namespace Filter {
		using namespace std;
		using namespace cv;

		IMPLEMENT_CVFILTER(AbsDiff);
		AbsDiff::AbsDiff() {
			setInputMatDesc("", "���͉摜1");
			defInputMat("subImage", "���͉摜2");
		}
		AbsDiff::~AbsDiff() { }
		void AbsDiff::execute() {
			const Mat& input_image = getInputMat();
			const Mat& sub_image = getInputMat("subImage");
			Mat& output = refOutputMat();
			cv::absdiff(input_image, sub_image, output);
		}
	}
}
