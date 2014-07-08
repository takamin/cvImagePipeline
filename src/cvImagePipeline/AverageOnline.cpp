#include "AverageOnline.h"
namespace cvImagePipeline {
	namespace Filter {
		using namespace std;
		using namespace cv;
		IMPLEMENT_CVFILTER(AverageOnline);
		AverageOnline::AverageOnline()
			: averageCount("averageCount", 30*60*10),
			buffer_count(0)
		{
			defParam(averageCount);
		}
		AverageOnline::~AverageOnline()
		{
		}
		template<class T> inline void average_online(const cv::Mat& input, cv::Mat& output, int buffer_count) {
			int input_channels = input.channels();
			for(int y = 0; y < input.rows; y++) {
				T* src = (T*)(input.data + input.step * y);
				T* ave = (T*)(output.data + output.step * y);
				for(int x = 0; x < input.cols; x++) {
					for(int i = 0; i < input_channels; i++) {
						ave[i] += (T)((double)(src[i] - ave[i]) / buffer_count);
					}
					src += input_channels;
					ave += input_channels;
				}
			}
		}
		void AverageOnline::execute() {
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
			switch(input_depth) {
			case CV_8U: average_online<unsigned char>(input_image, output_image, buffer_count); break;
			case CV_8S: average_online<char>(input_image, output_image, buffer_count); break;
			case CV_16U: average_online<unsigned short>(input_image, output_image, buffer_count); break;
			case CV_16S: average_online<short>(input_image, output_image, buffer_count); break;
			case CV_32F: average_online<float>(input_image, output_image, buffer_count); break;
			}
		}
	}
}
