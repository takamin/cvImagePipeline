#pragma once
#include "ImageProcessor.h"
namespace cvImagePipeline {
	namespace Filter {
		class __declspec(dllexport) ImageWindow : public ImageProcessor {
		public:
			DECLARE_CVFILTER;
		private:
			bool windowCreated;
			int frameNumber;
			int tick;
		public:
			TParam<std::string> windowName;
			TParam<bool> showFrameNumber;
			TParam<bool> showFPS;
		public:
			ImageWindow();
			ImageWindow(std::string windowName);
			void execute();
		};
	}
}
