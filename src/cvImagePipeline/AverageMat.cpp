#include "AverageMat.h"

namespace cvUtils {
	namespace Filter {
		using namespace std;
		using namespace cv;

		IMPLEMENT_CVFILTER(AverageMat);
		AverageMat::AverageMat()
			: averageCount("averageCount", 30),
			buffer_count(0)
		{
			defParam(averageCount);
			refOutputMat() = average_image;
			//setOutputMat(average_image);
		}
		AverageMat::~AverageMat()
		{
		}
		void AverageMat::execute() {

			// 1:[1];
			// 2:[1/2, 1/2];
			// 3:[1/3, 1/3, 1/3];
			// ...
			// 10:[1/10, 1/10, 1/10, ... 1/10];

			const Mat& input_image = getInputMat();
			const int max_count = averageCount;
			if(buffer_count >= max_count) {
				list<Mat>::iterator first_image = image_buffer.begin();
				cv::subtract(average_image, *first_image / buffer_count, average_image);
				cv::add(average_image, input_image / buffer_count, average_image);
				pushBuffer();
			} else {
				pushBuffer();
				average_image = Mat::zeros(input_image.rows, input_image.cols, input_image.type());
				list<Mat>::iterator bufimg = image_buffer.begin();
				for(int i = 0; i < buffer_count; i++) {
					cv::add(average_image, *bufimg / buffer_count, average_image);
					bufimg++;
				}
			}
			refOutputMat() = average_image;
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
