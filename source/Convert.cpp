#include "Convert.h"

namespace cvImagePipeline {
	namespace Filter {
		using namespace std;
		using namespace cv;

		IMPLEMENT_CVFILTER(Convert);
		Convert::Convert()
			: rtype("rtype", CV_MAKETYPE(CV_32F, 3)),
			alpha("alpha", 1.0),
			beta("beta", 1.0)
		{
			defParam(rtype);
			defParam(alpha);
			defParam(beta);
		}
		Convert::~Convert() { }
		void Convert::execute() {
			const Mat& input_image = getInputMat();
			Mat& output = refOutputMat();
			input_image.convertTo(output, rtype, alpha, beta);
		}
	}
}
