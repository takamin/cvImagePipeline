#include "SubMat.h"

namespace cvImagePipeline {
	namespace Filter {
		using namespace std;
		using namespace cv;

		IMPLEMENT_CVFILTER(SubMat);
		SubMat::SubMat() {
			setDescription("2画像の差分画像を出力。");
			undefInputMat("");
			defInputMat("src1", "入力画像1");
			defInputMat("src2", "入力画像2");
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
