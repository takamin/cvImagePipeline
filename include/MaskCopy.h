#pragma once
#include "ImageProcessor.h"
namespace cvImagePipeline {
	namespace Filter {
		class __declspec(dllexport) MaskCopy : public ImageProcessor
		{
		public:
			DECLARE_CVFILTER;
		public:
			MaskCopy();
			virtual ~MaskCopy();
			void execute();
		};
	}
}
