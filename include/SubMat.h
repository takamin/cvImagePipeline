#pragma once
#include "ImageProcessor.h"
namespace cvImagePipeline {
	namespace Filter {
		class SHARED SubMat : public ImageProcessor
		{
		public:
			DECLARE_CVFILTER;
		public:
			SubMat();
			virtual ~SubMat();
			void execute();
		};
	}
}
