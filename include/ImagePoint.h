#pragma once
#include "ImageProcessor.h"
namespace cvImagePipeline {
	namespace Filter {
		class __declspec(dllexport) ImagePoint : public ImageProcessor {
		public:
			DECLARE_CVFILTER;
			ImagePoint(){}
			~ImagePoint(){}
			void execute();
		};
	}
}