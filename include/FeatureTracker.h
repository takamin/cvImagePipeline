#pragma once
#include <list>
#include <opencv2/opencv.hpp>
#include "ImageProcessor.h"
#include "Labeler.h"
#include "TrackingObject.h"
#include "FeatureMatcher.h"
#pragma warning(disable:4251)

namespace cvUtils {
	namespace Filter {
		class __declspec(dllexport) FeatureTracker : public ImageProcessor {
		public:
			DECLARE_CVFILTER;
			FeatureTracker();
			virtual ~FeatureTracker();
			virtual void execute();
		public:
			//特徴点によってラベリングする場合の入力バッファ
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
			FeatureMatcher featureMatcher;	//特徴点抽出プロセッサ
			Buffer labeler_buffer;			//ラベリング用の入力バッファ
			Labeler labeler;				//ラベリングプロセッサ
		};
		#pragma warning(default:4251)
	}
}