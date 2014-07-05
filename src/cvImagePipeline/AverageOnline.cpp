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
			} else {
			}
			cv::Mat temp_image;
			cv::subtract(fmat, output_image, temp_image);
			for(int y = 0; y < temp_image.rows; y++) {
				float* p = (float*)(temp_image.data + temp_image.step * y);
				for(int x = 0; x < temp_image.cols; x++) {
					*p /= (float)(buffer_count + 1);
					*p++;
				}
			}
			cv::add(output_image, temp_image, output_image);
			//cv::Mat add_image;
			//cv::add(output_image, temp_image, add_image);
			//output_image = add_image;
			if(buffer_count < averageCount) {
				buffer_count++;
			}
		}
	}
}
