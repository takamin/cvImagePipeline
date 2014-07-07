#include "AverageOnline.h"
namespace cvImagePipeline {
	namespace Filter {
		using namespace std;
		using namespace cv;
		IMPLEMENT_CVFILTER(AverageOnline);
		AverageOnline::AverageOnline()
			: averageCount("averageCount", 60*10),
			buffer_count(0)
		{
			defParam(averageCount);
		}
		AverageOnline::~AverageOnline()
		{
		}
		void AverageOnline::execute() {
			const Mat& input_image = getInputMat();
			if(input_image.empty()) {
				return;
			}
			int input_channels = input_image.channels();
			int input_depth = input_image.depth();

			Mat float_image;
			if(input_depth == CV_32F) {
				float_image = input_image;
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
					float_image,
					CV_MAKETYPE(CV_32F, input_channels), alpha, beta);
			}

			Mat& output_image = refOutputMat();
			if(output_image.empty()) {
				output_image = Mat::zeros(
					input_image.rows, input_image.cols,
					CV_MAKETYPE(CV_32F, input_channels));
			}
			if(buffer_count < averageCount) {
				buffer_count++;
			}
			for(int y = 0; y < float_image.rows; y++) {
				float* src = (float*)(float_image.data + float_image.step * y);
				float* ave = (float*)(output_image.data + output_image.step * y);
				for(int x = 0; x < float_image.cols; x++) {
					for(int i = 0; i < input_channels; i++) {
						ave[i] += (src[i] - ave[i]) / buffer_count;
					}
					src += input_channels;
					ave += input_channels;
				}
			}
		}
	}
}
