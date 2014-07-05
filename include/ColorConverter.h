#pragma once
#include "ImageProcessor.h"
namespace cvImagePipeline {
	namespace Filter {
		class __declspec(dllexport) ColorConverter : public ImageProcessor {
		public:
			DECLARE_CVFILTER;
		private:
			TParam<int> cvtCode;
		public:
			ColorConverter();
			ColorConverter(int cvtCode);
			void execute();
		};
	}
}
