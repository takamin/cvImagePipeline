#include "AverageMat.h"

namespace cvImagePipeline {
	namespace Filter {
		using namespace std;
		using namespace cv;

		IMPLEMENT_CVFILTER(AverageMat);
		AverageMat::AverageMat()
			: averageCount("averageCount", 30),
			buffer_count(0)
		{
			defParam(averageCount);
		}
		AverageMat::~AverageMat()
		{
		}
		void AverageMat::execute() {
			const Mat& input_image = getInputMat();
			Mat& output_image = refOutputMat();
			const int max_count = averageCount;
			if(buffer_count >= max_count) {
				list<Mat>::iterator first_image = image_buffer.begin();
				cv::subtract(output_image, *first_image / buffer_count, output_image);
				cv::add(output_image, input_image / buffer_count, output_image);
				pushBuffer();
			} else {
				pushBuffer();
				output_image = Mat::zeros(input_image.rows, input_image.cols, input_image.type());
				list<Mat>::iterator bufimg = image_buffer.begin();
				for(int i = 0; i < buffer_count; i++) {
					cv::add(output_image, *bufimg / buffer_count, output_image);
					bufimg++;
				}
			}
		}

		void AverageMat::pushBuffer()
		{
			const int max_count = averageCount;
			const Mat& input_image = getInputMat();
			image_buffer.push_back(input_image.clone());
			buffer_count++;
			while(buffer_count > max_count) {
				image_buffer.erase(image_buffer.begin());
				buffer_count--;
			}
		}
	}
}
