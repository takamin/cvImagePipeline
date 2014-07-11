#include "ColorConverter.h"
using namespace std;
namespace cvImagePipeline {
	namespace Filter {
		///////////////////////////////////////////////////////////////////////////
		// class ColorConverter

		IMPLEMENT_CVFILTER(ColorConverter);
		ColorConverter::ColorConverter()
			: cvtCode("cvtCode", CV_BGR2GRAY)
			{ defParam(this->cvtCode); }
		ColorConverter::ColorConverter(int cvtCode)
			: cvtCode("cvtCode", cvtCode)
			{ defParam(this->cvtCode); }
		void ColorConverter::execute() {
			const cv::Mat& inputMat = getInputMat();
			cv::cvtColor(inputMat, refOutputMat(), cvtCode);
		}
	}
}
