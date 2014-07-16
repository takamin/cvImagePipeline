#pragma once
#include "ImageProcessor.h"
namespace cvImagePipeline {
	namespace Filter {
		class __declspec(dllexport) Threshold : public ImageProcessor
		{
		public:
			DECLARE_CVFILTER;
			TParam<double> thresh;
			TParam<double> maxval;
			TParam<int> type;
		public:
			Threshold();
			virtual ~Threshold();
			void execute();
		};
	}
}
