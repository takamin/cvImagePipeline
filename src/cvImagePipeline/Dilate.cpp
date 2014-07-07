#include "Dilate.h"

namespace cvImagePipeline {
	namespace Filter {
		using namespace std;
		using namespace cv;

		IMPLEMENT_CVFILTER(Dilate);
		Dilate::Dilate()
		: iterations("iterations", 1)
		{
			defParam(iterations);
		}
		Dilate::~Dilate() { }
		void Dilate::execute() {
			const Mat& input_image = getInputMat();
			if(input_image.empty()) {
				return;
			}
			Mat& output = refOutputMat();
			cv::dilate(input_image, output, cv::Mat(), cv::Point(-1,-1), iterations);
		}
	}
}
