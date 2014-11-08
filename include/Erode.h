#pragma once
#include "ImageProcessor.h"
namespace cvImagePipeline {
	namespace Filter {
		class SHARED Erode : public ImageProcessor
		{
		public:
			DECLARE_CVFILTER;
			TParam<int> iterations;
		public:
			Erode();
			virtual ~Erode();
			void execute();
		};
	}
}
