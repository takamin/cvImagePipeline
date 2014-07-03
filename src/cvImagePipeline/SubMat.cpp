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
				return;
			}
			if(sub_image.empty() ||
				input_image.rows != sub_image.rows
				|| input_image.cols != sub_image.cols
				|| input_image.type() != sub_image.type()
				|| input_image.channels() != sub_image.channels())
			{
				return;
			}
			Mat& output = refOutputMat();
			cv::subtract(input_image, sub_image, output);
		}
	}
}
