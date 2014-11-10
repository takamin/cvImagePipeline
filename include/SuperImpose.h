#pragma once
#include "ImageProcessor.h"
namespace cvImagePipeline {
	namespace Filter {
		class SHARED SuperImpose : public ImageProcessor {
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
#pragma warning(disable:4251)
			static cv::Mat empty_image;
#pragma warning(default:4251)
		};
	}
}
