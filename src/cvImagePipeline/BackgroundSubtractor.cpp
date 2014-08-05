#include "BackgroundSubtractor.h"

namespace cvImagePipeline {
	namespace Filter {
		IMPLEMENT_CVFILTER(BackgroundSubtractor);
		BackgroundSubtractor::BackgroundSubtractor()
			: backgroundSubtractor(0), buffer_count(0),
			algo("algo", GMG), averageCount("averageCount", 120)
		{
			defParam(averageCount);
			defParam(algo);
			::InitializeCriticalSection(&cs);
		}

		BackgroundSubtractor::~BackgroundSubtractor()
		{
			::DeleteCriticalSection(&cs);
			shutdown();
		}

		void BackgroundSubtractor::execute() {
			const cv::Mat& frame = getInputMat();
			if (!frame.empty()) {
				::EnterCriticalSection(&cs);
				if (buffer_count < averageCount) {
					buffer_count++;
				}
				if (backgroundSubtractor == 0) {
					create();
				}
				cv::Mat& mask = refOutputMat();
				switch (algo) {
				case GMG:
					(*(cv::BackgroundSubtractorGMG*)backgroundSubtractor).learningRate = 1.0 / buffer_count;
					(*(cv::BackgroundSubtractorGMG*)backgroundSubtractor)(frame, mask);
					break;
				case MOG:
					(*(cv::BackgroundSubtractorMOG*)backgroundSubtractor)(frame, mask);
					break;
				case MOG2:
					(*(cv::BackgroundSubtractorMOG2*)backgroundSubtractor)(frame, mask);
					break;
				}
				backgroundSubtractor->getBackgroundImage(backgroundImage);
				::LeaveCriticalSection(&cs);
			}
		}
		void BackgroundSubtractor::reset() {
			::EnterCriticalSection(&cs);
			shutdown();
			create();
			::LeaveCriticalSection(&cs);
		}
		const cv::Mat& BackgroundSubtractor::getBackgroundImage() const {
			return backgroundImage;
		}
		void BackgroundSubtractor::shutdown() {
			delete backgroundSubtractor;
			backgroundSubtractor = 0;
		}
		void BackgroundSubtractor::create() {
			buffer_count = 0;
			switch (algo) {
			case GMG:
				backgroundSubtractor = new cv::BackgroundSubtractorGMG();
				break;
			case MOG:
				backgroundSubtractor = new cv::BackgroundSubtractorMOG(averageCount, 3, 0.0);
				break;
			case MOG2:
				backgroundSubtractor = new cv::BackgroundSubtractorMOG2(averageCount, 50, true);
				break;
			}
		}
	}
}