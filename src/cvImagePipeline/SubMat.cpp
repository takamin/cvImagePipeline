#include "SubMat.h"

namespace cvUtils {
	namespace Filter {
		using namespace std;
		using namespace cv;

		IMPLEMENT_CVFILTER(SubMat);
		SubMat::SubMat() {
			defInputMat("subImage");
		}
		SubMat::~SubMat() { }
		void SubMat::execute() {
			const Mat& input_image = getInputMat();
			const Mat& sub_image = getInputMat("subImage");
			if(input_image.empty()) {
				refOutputMat() = input_image;
				return;
			}
			if(sub_image.empty() ||
				input_image.rows != sub_image.rows
				|| input_image.cols != sub_image.cols
				|| input_image.type() != sub_image.type()
				|| input_image.channels() != sub_image.channels())
			{
				refOutputMat() = input_image;
				return;
			}
			Mat output = Mat::zeros(input_image.rows, input_image.cols, input_image.type());
			cv::subtract(input_image, sub_image, output);
			refOutputMat() = output;
		}
	}
}
