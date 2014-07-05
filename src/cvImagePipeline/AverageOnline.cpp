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
		static double round(double r) { return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5); }
		template<class T> static void averageOnline(const Mat& new_frame, Mat& average, int count) {
			assert(new_frame.channels() == average.channels());
			int channels = new_frame.channels();
			for(int y = 0; y < new_frame.rows; y++) {
				T* src = (T*)(new_frame.data + new_frame.step * y);
				T* ave = (T*)(average.data + average.step * y);
				for(int x = 0; x < new_frame.cols; x++) {
					for(int i = 0; i < channels; i++) {
						ave[i] += (T)round(((double)src[i] - ave[i]) / count);
					}
					src += channels;
					ave += channels;
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
				output_image = Mat::zeros(input_image.rows, input_image.cols, input_image.type());
			}
			if(buffer_count < averageCount) {
				buffer_count++;
			}
			switch(input_depth) {
			case CV_8U: averageOnline<unsigned char>(input_image, output_image, buffer_count); break;
			case CV_8S: averageOnline<char>(input_image, output_image, buffer_count); break;
			case CV_16U: averageOnline<unsigned short>(input_image, output_image, buffer_count); break;
			case CV_16S: averageOnline<short>(input_image, output_image, buffer_count); break;
			case CV_32F: averageOnline<float>(input_image, output_image, buffer_count); break;
			}
		}
	}
}
