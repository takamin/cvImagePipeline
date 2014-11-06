#include "GaussianBlur.h"
using namespace std;
namespace cvImagePipeline {
	namespace Filter {
		///////////////////////////////////////////////////////////////////////////
		// class GaussianBlur

		IMPLEMENT_CVFILTER(GaussianBlur);
		GaussianBlur::GaussianBlur()
			:	kernelWidth("kernelWidth", 5),
				kernelHeight("kernelHeight", 5),
				sigmaX("sigmaX", 1.0),
				sigmaY("sigmaY", 0.0),
				borderType("borderType", cv::BORDER_DEFAULT)
		{
			defParam(this->kernelWidth);
			defParam(this->kernelHeight);
			defParam(this->sigmaX);
			defParam(this->sigmaY);
			defParam(this->borderType);
		}
		GaussianBlur::GaussianBlur(
			int kernelWidth, int kernelHeight, 
			double sigmaX, double sigmaY,
			int borderType)
			:	kernelWidth("kernelWidth", kernelWidth),
				kernelHeight("kernelHeight", kernelHeight),
				sigmaX("sigmaX", sigmaX),
				sigmaY("sigmaY", sigmaY),
				borderType("borderType", borderType)
		{
			defParam(this->kernelWidth);
			defParam(this->kernelHeight);
			defParam(this->sigmaX);
			defParam(this->sigmaY);
			defParam(this->borderType);
		}
		void GaussianBlur::execute() {
			const cv::Mat& inputMat = getInputMat();
			cv::GaussianBlur(
				inputMat, refOutputMat(),
				cv::Size(kernelWidth, kernelHeight),
				sigmaX, sigmaY, borderType);
		}
	}
}
