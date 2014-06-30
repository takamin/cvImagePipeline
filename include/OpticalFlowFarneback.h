#include <opencv2/opencv.hpp>
#include <opencv2/superres/optical_flow.hpp>
#include "ImageProcessor.h"

namespace cvUtils {
	namespace Filter {

		#pragma warning(disable:4251)

		///Farneback�̃A���S���Y���Ŗ��ȃI�v�e�B�J���t���[���v�Z����
		class __declspec(dllexport) OpticalFlowFarneback : public ImageProcessor {
		public:
			DECLARE_CVFILTER;
		private:
			TParam<double> pyr_scale;	//�摜�s���~�b�h�̃X�P�[���B0.5�ŉ��w�̃T�C�Y�������ɂȂ�B
			TParam<int> levels;			//�v�Z���x���H
			TParam<int> winsize;		//�E�B���h�E�T�C�Y�B���o����ŏ��̑傫���B
			TParam<int> iterations;		//�J��Ԃ��v�Z��
			TParam<int> poly_n;			//?
			TParam<double> poly_sigma;	//?
			TParam<int> flags;			//?

			cv::Mat prevMat;					//�O��̃t���[��
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
