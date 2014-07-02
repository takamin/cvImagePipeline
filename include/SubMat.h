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
		private:
#pragma warning(disable:4251)
			cv::Mat average_image;
#pragma warning(default:4251)
		};
	}
}
