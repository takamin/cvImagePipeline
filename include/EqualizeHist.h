#pragma once
#include "ImageProcessor.h"
namespace cvImagePipeline {
	namespace Filter {
		class SHARED EqualizeHist : public ImageProcessor
		{
		public:
			DECLARE_CVFILTER;
		public:
			EqualizeHist();
			~EqualizeHist();
			void execute();
		};
	}
}
