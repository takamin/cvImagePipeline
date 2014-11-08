#pragma once
#include <list>
#include <opencv2/opencv.hpp>
#include "ImageProcessor.h"
#include "Labeler.h"
#include "TrackingObject.h"
#include "FeatureMatcher.h"
#pragma warning(disable:4251)

namespace cvImagePipeline {
	namespace Filter {
		class __declspec(dllexport) FeatureTracker : public ImageProcessor {
		public:
			DECLARE_CVFILTER;
			FeatureTracker();
			virtual ~FeatureTracker();
			virtual void execute();
		public:
			//input buffer in labeling by feature
			class  __declspec(dllexport) Buffer : public Labeler::Buffer {
			public:
				void setFeature(
					int cols, int rows,
					const std::vector<cv::DMatch>& matches,
					const std::vector<cv::KeyPoint>& queryKeyPoints,
					const std::vector<cv::KeyPoint>& trainKeyPoints);
			};
			std::list<TrackingObject>& getObjectList() { return tracking_object_list; }
		private:
			std::list<TrackingObject> tracking_object_list;
			FeatureMatcher featureMatcher;	//processor to get feature
			Buffer labeler_buffer;			//input buffer for labeling
			Labeler labeler;				//labeling processor
		};
		#pragma warning(default:4251)
	}
}