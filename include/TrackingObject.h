#pragma once
#include <list>
#include <opencv2/opencv.hpp>
#include "ImageProcessor.h"
#include "LabeledObject.h"
#pragma warning(disable:4251)

namespace cvImagePipeline {
	struct SHARED TrackingObject {
		LabeledObject state;

		int life;
		int stopCount;
		bool end;

		TrackingObject(const LabeledObject& labeledObject);
		bool operator==(const TrackingObject& that);
		std::list<LabeledObject> history;
		void update(TrackingObject newItem);
		void noupdate();
		void putTo(std::ostream& os) const;

		std::vector<cv::DMatch> dmatches;
		void addMatch(const cv::DMatch& dmatch);

		void chooseMatches(
			const std::vector<cv::DMatch>& dmatches,
			const std::vector<cv::KeyPoint>& queryKeyPoints,
			const std::vector<cv::KeyPoint>& trainKeyPoints,
			std::vector<cv::DMatch>& matchesChose) const;

		TrackingObject createByFeature(
			const std::vector<cv::DMatch>& dmatches,
			const std::vector<cv::KeyPoint>& queryKeyPoints,
			const std::vector<cv::KeyPoint>& trainKeyPoints) const;
		bool updated;
	public:
		static void createTrackingObjects(
			const std::list<LabeledObject>& labeled_object_list,
			std::list<TrackingObject>& tracking_obj);
		static void setFeatures(
			std::list<TrackingObject>& trackingOjectListByFlow,
			const std::vector<cv::DMatch>& dmatches,
			const std::vector<cv::KeyPoint>& queryKeyPoints);
		static float calcMatchStatus(
			const TrackingObject& newObj,
			const TrackingObject& oldObj);
		static float calcMatchStatus(
			const TrackingObject& newObj,
			const TrackingObject& oldObj,
			const std::vector<cv::KeyPoint>& queryKeyPoints,
			const std::vector<cv::KeyPoint>& trainKeyPoints);
		static void eliminateExtraMatches(
			std::vector<cv::DMatch> matches,
			const std::list<TrackingObject>& tracking_object_list,
			const std::list<TrackingObject>& trackingOjectListByFlow);
		static void updateTrackingObjects(
			std::list<TrackingObject>& tracking_object_list,
			std::list<TrackingObject>& trackingOjectListByFlow,
			const std::vector<cv::DMatch>& matches);
		static void eraceOutMatches(
			std::list<TrackingObject>& tracking_object_list,
			const std::vector<cv::KeyPoint>& queryKeyPoints,
			const std::vector<cv::KeyPoint>& trainKeyPoints);
		static void draw(cv::Mat& outputMat, 
			const std::list<TrackingObject>&  tracking_object_list);
		static void draw(cv::Mat& outputMat, 
			const std::list<TrackingObject>&  tracking_object_list,
			const std::vector<cv::KeyPoint>& queryKeyPoints,
			const std::vector<cv::KeyPoint>& trainKeyPoints);
		static void draw(
			cv::Mat& image,
			const std::vector<cv::DMatch> dmatches,
			const std::vector<cv::KeyPoint>& queryKeyPoint,
			const std::vector<cv::KeyPoint>& trainKeyPoint,
			const cv::Scalar& color);
	private:
		void draw(
			cv::Mat& image,
			const std::vector<cv::KeyPoint>& queryKeyPoint,
			const std::vector<cv::KeyPoint>& trainKeyPoint,
			const cv::Scalar& color) const;

		void draw(
			cv::Mat& image,
			const cv::Scalar& color) const;
	};
	#pragma warning(default:4251)
}
 SHARED std::ostream& operator << (std::ostream&, const cvImagePipeline::TrackingObject& trackingObject);
