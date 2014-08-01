#pragma once
#include "ImageProcessor.h"
#define GMG 0
#define MOG 1
#define MOG2 2
#define _BackgroundSubtractor_ALG MOG
namespace cvImagePipeline {
	namespace Filter {
		class __declspec(dllexport) BackgroundSubtractor : public ImageProcessor
		{
		public:
			DECLARE_CVFILTER;
			TParam<int> algo;
			TParam<int> averageCount;
		public:
			BackgroundSubtractor();
			~BackgroundSubtractor();
			void execute();
		private:
			cv::BackgroundSubtractor* backgroundSubtractor;
			int buffer_count;
		};
	}
}