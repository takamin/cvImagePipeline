#pragma once
#include <list>
#include <opencv2/opencv.hpp>
#include "ImageProcessor.h"
#include "Labeler.h"
#include "TrackingObject.h"
#pragma warning(disable:4251)

namespace cvImagePipeline {
	namespace Filter {
		class __declspec(dllexport) FlowTracker : public ImageProcessor {
		public:
			DECLARE_CVFILTER;
			class  __declspec(dllexport) Buffer : public Labeler::Buffer {
			public:
				void drawFlow(const cv::Mat& flow, cv::Mat& dst, double labeling_vec_thrs);
				void setFlow(const cv::Mat& flow, double labeling_vec_thrs);
			};
		private:
			TParam<float> labeling_vec_thrs;
			Buffer labeler_buffer;
			Labeler labeler;
			std::list<TrackingObject> tracking_object_list;
		public:
			FlowTracker();
			virtual ~FlowTracker();
			virtual void execute();
			std::list<TrackingObject>& getObjectList() { return tracking_object_list; }
		private:
			static void createMatchScoreArray(
				std::vector<cv::DMatch>& matches,
				const std::list<TrackingObject>& tracking_object_list,
				const std::list<TrackingObject>& trackingOjectListByFlow);
		};
		#pragma warning(default:4251)
	}
}
