#pragma once
#include "ImageProcessor.h"
namespace cvImagePipeline {
	namespace Filter {
		class SHARED Flipper : public ImageProcessor {
		public:
			DECLARE_CVFILTER;
		private:
			TParam<int> flipDir;
		public:
			Flipper();
			Flipper(int flipDir);
			void execute();
		};
	}
}
