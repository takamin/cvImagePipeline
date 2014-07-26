#include "SubMat.h"

namespace cvImagePipeline {
	namespace Filter {
		using namespace std;
		using namespace cv;

		IMPLEMENT_CVFILTER(SubMat);
		SubMat::SubMat() {
			setInputMatDesc("", "���Z�����摜");
			defInputMat("subImage","���Z����摜");
		}
		SubMat::~SubMat() { }
		void SubMat::execute() {
			const Mat& input_image = getInputMat();
			const Mat& sub_image = getInputMat("subImage");
			Mat& output = refOutputMat();
			cv::subtract(input_image, sub_image, output);
		}
	}
}
