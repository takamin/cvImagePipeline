#include <opencv2/opencv.hpp>
#include <opencv2/superres/optical_flow.hpp>
#include "ImageProcessor.h"

namespace cvImagePipeline {
	namespace Filter {

		#pragma warning(disable:4251)

		///Farnebackのアルゴリズムで密なオプティカルフローを計算する
		class __declspec(dllexport) OpticalFlowFarneback : public ImageProcessor {
		public:
			DECLARE_CVFILTER;
		private:
			TParam<double> pyr_scale;	//画像ピラミッドのスケール。0.5で下層のサイズが半分になる。
			TParam<int> levels;			//計算レベル？
			TParam<int> winsize;		//ウィンドウサイズ。検出する最小の大きさ。
			TParam<int> iterations;		//繰り返し計算回数
			TParam<int> poly_n;			//?
			TParam<double> poly_sigma;	//?
			TParam<int> flags;			//?

			cv::Mat prevMat;					//前回のフレーム
		public:
			OpticalFlowFarneback();
			void reset();
			void execute();
		private:
			void calc(const cv::Mat& prev, const cv::Mat& next, cv::Mat& flow);
		};

		class __declspec(dllexport) OpticalFlowVisualizer : public ImageProcessor {
		public:
			void execute();
		};
		#pragma warning(default:4251)
	}
}
