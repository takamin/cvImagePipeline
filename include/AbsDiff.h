#pragma once
#include "ImageProcessor.h"
namespace cvImagePipeline {
	namespace Filter {
		class __declspec(dllexport) AbsDiff : public ImageProcessor
		{
		public:
			DECLARE_CVFILTER;
		public:
			AbsDiff();
			virtual ~AbsDiff();
			void execute();
		};
	}
}
