#include "Flipper.h"
using namespace std;
namespace cvImagePipeline {
	namespace Filter {
		///////////////////////////////////////////////////////////////////////////
		// class Flipper

		IMPLEMENT_CVFILTER(Flipper);
		Flipper::Flipper()
			: flipDir("flipDir", 1)
			{ defParam(this->flipDir); }
		Flipper::Flipper(int flipDir)
			: flipDir("flipDir", flipDir)
			{ defParam(this->flipDir); }
		void Flipper::execute() {
			const cv::Mat& inputMat = getInputMat();
			cv::flip(inputMat, refOutputMat(), flipDir);
		}
	}
}