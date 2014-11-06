#include "ImagePoint.h"
#include <typeinfo>
#include <time.h>
using namespace std;
namespace cvImagePipeline {
	namespace Filter {
		///////////////////////////////////////////////////////////////////////////
		// class ImagePoint
		// always outputs the input image

		IMPLEMENT_CVFILTER(ImagePoint);
		void ImagePoint::execute(){
			refOutputMat() = getInputMat();
		}
	}
}