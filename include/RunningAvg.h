#pragma once
#include "ImageProcessor.h"
namespace cvImagePipeline {
	namespace Filter {
		/*md*
		 *
		 * outputs a average image.
		 *
		 * input depth must be CV_32F or CV_64F.
		 *
		 * output depth is same to input.
		 *
		 * property `buffer_count` is averaging count.
		 *
		 * using `cvRunningAvg()`.
		 * the parameter `alpha` is set as `(1.0 / buffer_count)`
		 * `buffer_count` is incremented to property `averageCount`
		 *
		 */
		class SHARED RunningAvg : public ImageProcessor
		{
		public:
			DECLARE_CVFILTER;
			//md/ average count.
			TParam<int> averageCount;
		public:
			RunningAvg();
			virtual ~RunningAvg();
			virtual void reset();
			void execute();
		private:
			int buffer_count;
		};
	}
}
