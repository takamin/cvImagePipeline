#pragma once
#include "ImageProcessor.h"
namespace cvUtils {
	namespace Filter {
		class __declspec(dllexport) AverageMat : public ImageProcessor
		{
		public:
			DECLARE_CVFILTER;
			TParam<int> averageCount;
		public:
			AverageMat();
			virtual ~AverageMat();
			void execute();
		private:
#pragma warning(disable:4251)
			cv::Mat average_image;
			std::list<cv::Mat> image_buffer;
#pragma warning(default:4251)
			int buffer_count;
		private:
			void pushBuffer();
		};
	}
}
