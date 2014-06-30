#pragma once
#include "ImageProcessor.h"
namespace cvUtils {
	namespace Filter {
		class __declspec(dllexport) EqualizeHist : public ImageProcessor
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
