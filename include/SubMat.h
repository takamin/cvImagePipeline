#pragma once
#include "ImageProcessor.h"
namespace cvUtils {
	namespace Filter {
		class __declspec(dllexport) SubMat : public ImageProcessor
		{
		public:
			DECLARE_CVFILTER;
		public:
			SubMat();
			virtual ~SubMat();
			void execute();
		};
	}
}
