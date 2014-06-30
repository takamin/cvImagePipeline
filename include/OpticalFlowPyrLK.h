#include <opencv2/opencv.hpp>
#include <opencv2/superres/optical_flow.hpp>
#include "ImageProcessor.h"

namespace cvUtils {
	namespace Filter {

		#pragma warning(disable:4251)
		///PyrLKで疎なオプティカルフローを計算する
		class __declspec(dllexport) OpticalFlowPyrLK : public ImageProcessor {
		public:
			DECLARE_CVFILTER;
		private:
			cv::Mat prevMat; //前回のフレーム
		public:
			OpticalFlowPyrLK();
			void reset();
			void execute();
		};
		#pragma warning(default:4251)
	}
}
