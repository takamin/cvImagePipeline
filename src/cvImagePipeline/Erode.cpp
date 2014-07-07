#include "Erode.h"

namespace cvImagePipeline {
	namespace Filter {
		using namespace std;
		using namespace cv;

		IMPLEMENT_CVFILTER(Erode);
		Erode::Erode()
		: iterations("iterations", 1)
		{
			defParam(iterations);
		}
		Erode::~Erode() { }
		void Erode::execute() {
			const Mat& input_image = getInputMat();
			if(input_image.empty()) {
				return;
			}
			Mat& output = refOutputMat();
			cv::erode(input_image, output, cv::Mat(), cv::Point(-1,-1), iterations);
		}
	}
}
