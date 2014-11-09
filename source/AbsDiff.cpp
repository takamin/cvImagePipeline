#include "AbsDiff.h"

namespace cvImagePipeline {
	namespace Filter {
		using namespace std;
		using namespace cv;

		IMPLEMENT_CVFILTER(AbsDiff);
		AbsDiff::AbsDiff() {
			setDescription("difference between 2 images. output=abs(src1-src2)");
			undefInputMat("");
			defInputMat("src1", "src1");
			defInputMat("src2", "src2");
		}
		AbsDiff::~AbsDiff() { }
		void AbsDiff::execute() {
			const Mat& src1 = getInputMat("src1");
			const Mat& src2 = getInputMat("src2");
			if (src1.empty() || src2.empty()) {
				return;
			}
			Mat& dst = refOutputMat();
			cv::absdiff(src1, src2, dst);
		}
	}
}
