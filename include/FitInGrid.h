#pragma once
#include "ImageProcessor.h"
namespace cvImagePipeline {
	namespace Filter {
		class __declspec(dllexport) FitInGrid : public ImageProcessor {
		public:
			DECLARE_CVFILTER;
			TParam<int> width;
			TParam<int> height;
			TParam<int> cols;
			TParam<int> rows;
		public:
			FitInGrid();
			~FitInGrid();
			void execute();
		private:
			void onPropertyChange(Property& property);
			void onInputMatConnected(const std::string& inputMatName);
			void refresh();
		private:
			std::vector<cv::Mat*> dynInputMat;
			int gridW;
			int gridH;
		};
	}
}
