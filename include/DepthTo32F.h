#pragma once
#include "ImageProcessor.h"
namespace cvImagePipeline {
	namespace Filter {
		class __declspec(dllexport) DepthTo32F : public ImageProcessor
		{
		public:
			DECLARE_CVFILTER;
		public:
			DepthTo32F();
			virtual ~DepthTo32F();
			void execute();
		};
	}
}
