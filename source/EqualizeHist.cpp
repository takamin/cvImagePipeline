#include "EqualizeHist.h"

namespace cvImagePipeline {
	namespace Filter {
		
		IMPLEMENT_CVFILTER(EqualizeHist);

		EqualizeHist::EqualizeHist()
		{
		}
		EqualizeHist::~EqualizeHist()
		{
		}
		void EqualizeHist::execute() {
			const cv::Mat& in = getInputMat();
			cv::Mat& out = refOutputMat();
			cv::equalizeHist(in, out);
		}
	}
}