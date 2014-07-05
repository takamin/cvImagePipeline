#pragma once
#include "ImageProcessor.h"
namespace cvImagePipeline {
	namespace Filter {
		class __declspec(dllexport) Resizer : public ImageProcessor {
		public:
			DECLARE_CVFILTER;
			TParam<int> width;
			TParam<int> height;
		public:
			Resizer();
			Resizer(int width, int height);
			void execute();
		};
	}
}
