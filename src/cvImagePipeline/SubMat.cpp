#include "SubMat.h"

namespace cvImagePipeline {
	namespace Filter {
		using namespace std;
		using namespace cv;

		IMPLEMENT_CVFILTER(SubMat);
		SubMat::SubMat() {
			setDescription("2‰æ‘œ‚Ì·•ª‰æ‘œ‚ğo—ÍB");
			undefInputMat("");
			defInputMat("src1", "“ü—Í‰æ‘œ1");
			defInputMat("src2", "“ü—Í‰æ‘œ2");
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
