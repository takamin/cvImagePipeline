#include "DepthTo32F.h"
namespace cvImagePipeline {
	namespace Filter {
		using namespace std;
		using namespace cv;
		IMPLEMENT_CVFILTER(DepthTo32F);
		DepthTo32F::DepthTo32F()
		{
		}
		DepthTo32F::~DepthTo32F()
		{
		}
		void DepthTo32F::execute() {
			const Mat& input_image = getInputMat();
			if(input_image.empty()) {
				return;
			}
			Mat& output_image = refOutputMat();
			int input_depth = input_image.depth();
			if(input_depth == CV_32F) {
				output_image = input_image;
			} else {
				double alpha = 1.0;
				double beta = 0.0;
				switch(input_depth) {
				case CV_8U: alpha = 1.0 / 256; beta = 0.0; break;
				case CV_8S: alpha = 1.0 / 128; beta = 0.5; break;
				case CV_16U: alpha = 1.0 / 0x1000; beta = 0.0; break;
				case CV_16S: alpha = 1.0 / 0x800;  beta = 0.5; break;
				}
				input_image.convertTo(
					output_image,
					CV_MAKETYPE(CV_32F, input_image.channels()),
					alpha, beta);
			}
		}
	}
}
