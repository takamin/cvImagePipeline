#pragma once
#include "ImageProcessor.h"
namespace cvImagePipeline {
	namespace Filter {
		class SHARED AbsDiff : public ImageProcessor
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
