#pragma once
#include <list>
#include <opencv2/opencv.hpp>
#include "ImageProcessor.h"
#include "Labeling.h"
#include "TrackingObject.h"
#pragma warning(disable:4251)

namespace cvUtils {
	namespace Filter {
		class __declspec(dllexport) FeatureMatcher : public ImageProcessor
		{
		public:
			DECLARE_CVFILTER;
			static cv::Ptr<cv::FeatureDetector> featureDetector;
			static cv::Ptr<cv::DescriptorExtractor> extractor;
			static cv::Ptr<cv::DescriptorMatcher> matcher;
		public:
			FeatureMatcher();
			virtual ~FeatureMatcher();
			virtual void execute();
			const std::vector<cv::DMatch>& getMatches() const { return dmatches; }
			const std::vector<cv::KeyPoint>& getQueryKeyPoints() const { return queryKeyPoints; }
			const std::vector<cv::KeyPoint>& getTrainKeyPoints() const { return trainKeyPoints; }
			void draw(cv::Mat& image, cv::Scalar& color);
		private:
			static void prepareAlgorithm();
			static void matchFeatures(
				std::vector<cv::KeyPoint>& queryKeyPoints, cv::Mat& queryDescriptors,
				std::vector<cv::KeyPoint>& trainKeyPoints, cv::Mat& trainDescriptors,
				std::vector<cv::DMatch>& dmatches);
			static void measureDistance(
				std::vector<cv::DMatch>& matches,
				const std::vector<cv::KeyPoint>& queryKeyPoints,
				const std::vector<cv::KeyPoint>& trainKeyPoints);
			static void filterByDistanceStdDev(
				const std::vector<cv::DMatch>& matches,
				float range_factor,
				std::vector<cv::DMatch>& dmatches);
		private:
			std::vector<cv::DMatch> dmatches;
			std::vector<cv::KeyPoint> queryKeyPoints;
			std::vector<cv::KeyPoint> trainKeyPoints;
			cv::Mat queryDescriptors; 
			cv::Mat trainDescriptors;
		};
	}
}
