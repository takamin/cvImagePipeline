#include "AbsDiff.h"

namespace cvImagePipeline {
	namespace Filter {
		using namespace std;
		using namespace cv;

		IMPLEMENT_CVFILTER(AbsDiff);
		AbsDiff::AbsDiff() {
			setInputMatDesc("", "入力画像1");
			defInputMat("subImage", "入力画像2");
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
