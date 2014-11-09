#include <opencv2/opencv.hpp>
#include <opencv2/superres/optical_flow.hpp>
#include "ImageProcessor.h"

namespace cvImagePipeline {
	namespace Filter {

		#pragma warning(disable:4251)

		///cacl optical flow by Farneback algorithm
		class SHARED OpticalFlowFarneback : public ImageProcessor {
		public:
			DECLARE_CVFILTER;
		private:
			TParam<double> pyr_scale;	//scale of image pyramid. 0.5 : half
			TParam<int> levels;
			TParam<int> winsize;		//detection window size
			TParam<int> iterations;	
			TParam<int> poly_n;	
			TParam<double> poly_sigma;
			TParam<int> flags;

			cv::Mat prevMat;
		public:
			OpticalFlowFarneback();
			void reset();
			void execute();
		private:
			void calc(const cv::Mat& prev, const cv::Mat& next, cv::Mat& flow);
		};

		class SHARED OpticalFlowVisualizer : public ImageProcessor {
		public:
			void execute();
		};
		#pragma warning(default:4251)
	}
}
