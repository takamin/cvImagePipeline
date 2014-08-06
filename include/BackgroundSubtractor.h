#pragma once
#include <Windows.h>
#include <hash_map>
#include "ImageProcessor.h"
#define GMG 0
#define MOG 1
#define MOG2 2
#define _BackgroundSubtractor_ALG MOG
namespace cvImagePipeline {
	namespace Filter {
		class __declspec(dllexport) BackgroundSubtractor : public ImageProcessor
		{
			CRITICAL_SECTION cs;
		public:
			DECLARE_CVFILTER;
			enum Algorithm { AlgorithmGMG = GMG, AlgorithmMOG = MOG, AlgorithmMOG2 = MOG2 };
			TParam<Algorithm> algorithm;
			TParam<int> averageCount;
			TParam<bool> MOG2_shadowDetection;
		public:
			BackgroundSubtractor();
			~BackgroundSubtractor();
			void execute();
			void reset();
			const cv::Mat& getBackgroundImage() const;
		private:
			cv::BackgroundSubtractor* backgroundSubtractor;
			int buffer_count;
#pragma warning(disable:4251)
			cv::Mat backgroundImage;
#pragma warning(default:4251)
		private:
			void shutdown();
			void create();
			void (BackgroundSubtractor::*executeAlgorithm)(const cv::Mat&, cv::Mat& mask);
			void executeGMG(const cv::Mat& frame, cv::Mat& mask);
			void executeMOG(const cv::Mat& frame, cv::Mat& mask);
			void executeMOG2(const cv::Mat& frame, cv::Mat& mask);
		};
	}
}
__declspec(dllexport) std::istringstream& operator >> (
	std::istringstream& is,
	cvImagePipeline::Filter::BackgroundSubtractor::Algorithm& value);
__declspec(dllexport) std::stringstream& operator << (
	std::stringstream& ss, 
	const cvImagePipeline::Filter::BackgroundSubtractor::Algorithm& value);

