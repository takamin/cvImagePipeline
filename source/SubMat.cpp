#include "SubMat.h"

namespace cvImagePipeline {
	namespace Filter {
		using namespace std;
		using namespace cv;

		IMPLEMENT_CVFILTER(SubMat);
		SubMat::SubMat() {
			setDescription("output subtract image. output=(src1-src2)");
			undefInputMat("");
			defInputMat("src1", "source 1");
			defInputMat("src2", "source 2");
		}
		SubMat::~SubMat() { }
		void SubMat::execute() {
			const Mat& src1 = getInputMat("src1");
			const Mat& src2 = getInputMat("src2");
			if (src1.empty() || src2.empty()) {
				return;
			}
			Mat& dst = refOutputMat();
			cv::subtract(src1, src2, dst);
		}
	}
}
