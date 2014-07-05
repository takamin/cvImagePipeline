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
			//�����_�ɂ���ă��x�����O����ꍇ�̓��̓o�b�t�@
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
			FeatureMatcher featureMatcher;	//�����_���o�v���Z�b�T
			Buffer labeler_buffer;			//���x�����O�p�̓��̓o�b�t�@
			Labeler labeler;				//���x�����O�v���Z�b�T
		};
		#pragma warning(default:4251)
	}
}