#pragma once
#include "ImageProcessor.h"
namespace cvImagePipeline {
	namespace Filter {
		class __declspec(dllexport) SuperImpose : public ImageProcessor {
		public:
			DECLARE_CVFILTER;
			TParam<int> left;
			TParam<int> top;
			TParam<int> width;
			TParam<int> height;
		public:
			SuperImpose();
			~SuperImpose(){}
			void execute();
		private:
			static cv::Mat empty_image;
		};
	}
}