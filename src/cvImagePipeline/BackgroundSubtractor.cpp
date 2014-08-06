#include "BackgroundSubtractor.h"

namespace cvImagePipeline {
	namespace Filter {
		IMPLEMENT_CVFILTER(BackgroundSubtractor);
		BackgroundSubtractor::BackgroundSubtractor()
			: buffer_count(0), executeAlgorithm(0), backgroundSubtractor(0),
			algorithm("algorithm", AlgorithmMOG2),
			averageCount("averageCount", 120),
			MOG2_shadowDetection("MOG2_shadowDetection", true)
		{
			defParam(algorithm);
			defParam(averageCount);
			defParam(MOG2_shadowDetection);
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
				(this->*executeAlgorithm)(frame, mask);
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
			switch (algorithm) {
			case GMG:
				backgroundSubtractor = new cv::BackgroundSubtractorGMG();
				executeAlgorithm = &BackgroundSubtractor::executeGMG;
				break;
			case MOG:
				backgroundSubtractor = new cv::BackgroundSubtractorMOG(averageCount, 3, 0.0);
				executeAlgorithm = &BackgroundSubtractor::executeMOG;
				break;
			case MOG2:
				backgroundSubtractor = new cv::BackgroundSubtractorMOG2(averageCount, 50, MOG2_shadowDetection);
				executeAlgorithm = &BackgroundSubtractor::executeMOG2;
				break;
			}
		}
		void BackgroundSubtractor::executeGMG(const cv::Mat& frame, cv::Mat& mask) {
			(*(cv::BackgroundSubtractorGMG*)backgroundSubtractor).learningRate = 1.0 / buffer_count;
			(*(cv::BackgroundSubtractorGMG*)backgroundSubtractor)(frame, mask);
		}
		void BackgroundSubtractor::executeMOG(const cv::Mat& frame, cv::Mat& mask) {
			(*(cv::BackgroundSubtractorMOG*)backgroundSubtractor)(frame, mask);
		}
		void BackgroundSubtractor::executeMOG2(const cv::Mat& frame, cv::Mat& mask) {
			(*(cv::BackgroundSubtractorMOG2*)backgroundSubtractor)(frame, mask);
			if (MOG2_shadowDetection) {
				cv::threshold(mask, mask, 150, 255, CV_THRESH_BINARY);
			}
		}
	}
}
using namespace cvImagePipeline::Filter;
std::istringstream& operator >> (
	std::istringstream& is,
	BackgroundSubtractor::Algorithm& value)
{
	std::string str;
	is >> str;
	if (str == "AlgorithmGMG") {
		value = BackgroundSubtractor::AlgorithmGMG;
	}
	else if (str == "AlgorithmMOG") {
		value = BackgroundSubtractor::AlgorithmMOG;
	}
	else if (str == "AlgorithmMOG2") {
		value = BackgroundSubtractor::AlgorithmMOG2;
	}
	return is;
}


std::stringstream& operator << (
	std::stringstream& ss,
	const BackgroundSubtractor::Algorithm& value)
{
	std::string str;
	switch (value) {
	case BackgroundSubtractor::AlgorithmGMG:
		str = "AlgorithmGMG";
		break;
	case BackgroundSubtractor::AlgorithmMOG:
		str = "AlgorithmMOG";
		break;
	case BackgroundSubtractor::AlgorithmMOG2:
		str = "AlgorithmMOG2";
		break;
	}
	ss << str;
	return ss;
}

