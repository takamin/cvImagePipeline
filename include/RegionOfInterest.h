#pragma once
#include "ImageProcessor.h"
namespace cvImagePipeline {
	namespace Filter {
		class RegionOfInterest : public ImageProcessor
		{
		public:
			DECLARE_CVFILTER;
			TParam<int> left;
			TParam<int> top;
			TParam<int> width;
			TParam<int> height;
		public:
			RegionOfInterest();
			~RegionOfInterest();
			void execute();
		};
	}
};
