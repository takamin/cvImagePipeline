#include "SubMat.h"

namespace cvImagePipeline {
	namespace Filter {
		using namespace std;
		using namespace cv;

		IMPLEMENT_CVFILTER(SubMat);
		SubMat::SubMat() {
			setDescription("2�摜�̍����摜���o�́B");
			undefInputMat("");
			defInputMat("src1", "���͉摜1");
			defInputMat("src2", "���͉摜2");
		}
		SubMat::~SubMat() { }
		void SubMat::execute() {
			const Mat& src1 = getInputMat("src1");
			const Mat& src2 = getInputMat("src2");
			Mat& dst = refOutputMat();
			cv::subtract(src1, src2, dst);
		}
	}
}
