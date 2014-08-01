#include "BackgroundSubtractor.h"

namespace cvImagePipeline {
	namespace Filter {
		IMPLEMENT_CVFILTER(BackgroundSubtractor);
		BackgroundSubtractor::BackgroundSubtractor()
			: backgroundSubtractor(0)
		{
			backgroundSubtractor = new cv::BackgroundSubtractorGMG();
		}

		BackgroundSubtractor::~BackgroundSubtractor()
		{
			delete backgroundSubtractor;
		}

		void BackgroundSubtractor::execute() {
			const cv::Mat& frame = getInputMat();
			if (!frame.empty()) {
				cv::Mat& mask = refOutputMat();
				((cv::BackgroundSubtractorGMG*)backgroundSubtractor)->operator()(frame, mask);
			}
		}
	}
}