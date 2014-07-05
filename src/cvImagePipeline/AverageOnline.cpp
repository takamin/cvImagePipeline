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
			Mat fmat = Mat::zeros(input_image.rows, input_image.cols, CV_32FC1);
			input_image.convertTo(fmat, fmat.type(), 1.0 / 256);
			Mat& output_image = refOutputMat();
			if(output_image.empty()) {
				output_image = Mat::zeros(input_image.rows, input_image.cols, CV_32FC1);
			}
			if(buffer_count < averageCount) {
				buffer_count++;
			}
			for(int y = 0; y < fmat.rows; y++) {
				float* src = (float*)(fmat.data + fmat.step * y);
				float* ave = (float*)(output_image.data + output_image.step * y);
				for(int x = 0; x < fmat.cols; x++) {
					*ave += (*src - *ave) / buffer_count;
					src++;
					ave++;
				}
			}
		}
	}
}
