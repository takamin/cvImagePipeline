#include "Resizer.h"
using namespace std;
namespace cvImagePipeline {
	namespace Filter {
		///////////////////////////////////////////////////////////////////////////
		// class Resizer

		IMPLEMENT_CVFILTER(Resizer);
		Resizer::Resizer()
			: width("width", 640), height("height", 480)
		{
			defParam(this->width);
			defParam(this->height);
		}
		Resizer::Resizer(int width, int height)
			: width("width", width), height("height", height)
		{
			defParam(this->width);
			defParam(this->height);
		}
		void Resizer::execute() {
			const cv::Mat& inputMat = getInputMat();
			cv::resize(inputMat, refOutputMat(), cv::Size(width, height));
		}
	}
}
