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
			Mat& dst = refOutputMat();
            if (src1.empty()) {
                if (src2.empty()) {
                    return;
                } else {
                    dst = src2;
                }
            } else {
                if (src2.empty()) {
                    dst = src1;
                } else {
			        cv::absdiff(src1, src2, dst);
                }
            }
		}
	}
}
