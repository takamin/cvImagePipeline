#pragma once
#include "ImageProcessor.h"
namespace cvImagePipeline {
	namespace Filter {
		class __declspec(dllexport) BackgroundSubtractor : public ImageProcessor
		{
		public:
			DECLARE_CVFILTER;
			BackgroundSubtractor();
			~BackgroundSubtractor();
			void execute();
		private:
			cv::BackgroundSubtractor* backgroundSubtractor;
		};
	}
}