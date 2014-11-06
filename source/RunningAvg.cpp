#include "RunningAvg.h"
namespace cvImagePipeline {
	namespace Filter {
		using namespace std;
		using namespace cv;
		IMPLEMENT_CVFILTER(RunningAvg);
		RunningAvg::RunningAvg()
			: averageCount("averageCount", 60*10),
			buffer_count(0)
		{
			setDescription("���͉摜�̒������ω摜���o�͂���B");
			defParam(averageCount);
		}
		RunningAvg::~RunningAvg()
		{
		}
		void RunningAvg::reset() {
			buffer_count = 0;
			refOutputMat() = cv::Mat();
		}
		void RunningAvg::execute() {
			const Mat& input_image = getInputMat();
			if(input_image.empty()) {
				return;
			}
			int input_channels = input_image.channels();
			int input_depth = input_image.depth();

			Mat& output_image = refOutputMat();
			if(output_image.empty()) {
				output_image = Mat::zeros(
					input_image.rows, input_image.cols,
					input_image.type());
			}
			if(buffer_count < averageCount) {
				buffer_count++;
			}
			IplImage iplIn = input_image;
			IplImage iplOut = output_image;
			cvRunningAvg(&iplIn, &iplOut, 1.0 / buffer_count);
		}
	}
}
