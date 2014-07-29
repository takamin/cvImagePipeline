#pragma once
#include "ImageProcessor.h"
namespace cvImagePipeline {
	namespace Filter {
		class __declspec(dllexport) VideoCapture : public ImageProcessor {
		public:
			DECLARE_CVFILTER;
			TParam<int> deviceIndex;
			TParam<std::string> filename;
			TParam<double> width;
			TParam<double> height;
			TParam<int> startFrame;
			TParam<int> stopFrame;
			TParam<bool> captureStart;
			TParam<bool> captureEmpty;
		private:
#pragma warning(disable:4251)
			cv::VideoCapture videoCapture;
#pragma warning(default:4251)
			int frameNumber;
		public:
			VideoCapture();
			~VideoCapture();
			bool open(int deviceIndex);
			bool open(const std::string& filename);
			bool capture();
			void execute();
		};
	}
}
