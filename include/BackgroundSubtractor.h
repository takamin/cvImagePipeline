#pragma once
#include <Windows.h>
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
			TParam<int> algo;
			TParam<int> averageCount;
		public:
			BackgroundSubtractor();
			~BackgroundSubtractor();
			void execute();
			void reset();
			const cv::Mat& getBackgroundImage() const;
		private:
			cv::BackgroundSubtractor* backgroundSubtractor;
			int buffer_count;
			cv::Mat backgroundImage;
		private:
			void shutdown();
			void create();
		};
	}
}