#include "AbsDiff.h"

namespace cvImagePipeline {
	namespace Filter {
		using namespace std;
		using namespace cv;

		IMPLEMENT_CVFILTER(AbsDiff);
		AbsDiff::AbsDiff() {
			setDescription("2‰æ‘œ‚Ì·•ª(â‘Î’l)‰æ‘œ‚ğo—ÍB");
			undefInputMat("");
			defInputMat("src1", "“ü—Í‰æ‘œ1");
			defInputMat("src2", "“ü—Í‰æ‘œ2");
		}
		AbsDiff::~AbsDiff() { }
		void AbsDiff::execute() {
			const Mat& src1 = getInputMat("src1");
			const Mat& src2 = getInputMat("src2");
			Mat& dst = refOutputMat();
			cv::absdiff(src1, src2, dst);
		}
	}
}
