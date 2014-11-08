#pragma once
#include "ImageProcessor.h"
namespace cvImagePipeline {
	namespace Filter {
		class SHARED GaussianBlur : public ImageProcessor {
		public:
			DECLARE_CVFILTER;
		private:
			TParam<int> kernelWidth;
			TParam<int> kernelHeight;
			TParam<double> sigmaX;
			TParam<double> sigmaY;
			TParam<int> borderType;
		public:
			GaussianBlur();
			GaussianBlur(
				int kernelWidth, int kernelHeight,
				double sigmaX, double sigmaY = 0.0, int borderType = cv::BORDER_DEFAULT);
			void execute();
		};
	}
}
