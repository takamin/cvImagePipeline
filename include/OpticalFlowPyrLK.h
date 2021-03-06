#include <opencv2/opencv.hpp>
#include <opencv2/superres/optical_flow.hpp>
#include "ImageProcessor.h"

namespace cvImagePipeline {
	namespace Filter {

		#pragma warning(disable:4251)
		class SHARED OpticalFlowPyrLK : public ImageProcessor {
		public:
			DECLARE_CVFILTER;
		private:
			cv::Mat prevMat;
		public:
			OpticalFlowPyrLK();
			void reset();
			void execute();
		};
		#pragma warning(default:4251)
	}
}
