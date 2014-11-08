#pragma once
#include "ImageProcessor.h"
namespace cvImagePipeline {
	namespace Filter {
		class SHARED Dilate : public ImageProcessor
		{
		public:
			DECLARE_CVFILTER;
			TParam<int> iterations;
		public:
			Dilate();
			virtual ~Dilate();
			void execute();
		};
	}
}
