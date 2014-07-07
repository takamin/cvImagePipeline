#pragma once
#include "ImageProcessor.h"
namespace cvImagePipeline {
	namespace Filter {
		class __declspec(dllexport) Convert : public ImageProcessor
		{
		public:
			DECLARE_CVFILTER;
			TParam<int> rtype;
			TParam<double> alpha;
			TParam<double> beta;
		public:
			Convert();
			virtual ~Convert();
			void execute();
		};
	}
}
