#include "AbsDiff.h"

namespace cvImagePipeline {
	namespace Filter {
		using namespace std;
		using namespace cv;

		IMPLEMENT_CVFILTER(AbsDiff);
		AbsDiff::AbsDiff() {
			setDescription("2�摜�̍���(��Βl)�摜���o�́B");
			undefInputMat("");
			defInputMat("src1", "���͉摜1");
			defInputMat("src2", "���͉摜2");
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
