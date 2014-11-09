#include "FeatureMatcher.h"
//#define MATCH_SIMGLE
//#define NO_OMIT_ILLEGAL_MATCH
namespace cvImagePipeline {
	namespace Filter {
		using namespace std;
		using namespace cv;
		Ptr<FeatureDetector> FeatureMatcher::featureDetector = 0;
		Ptr<DescriptorExtractor> FeatureMatcher::extractor = 0;
		Ptr<DescriptorMatcher> FeatureMatcher::matcher = 0;
		IMPLEMENT_CVFILTER(FeatureMatcher);
		FeatureMatcher::FeatureMatcher()
		{
		}
		FeatureMatcher::~FeatureMatcher()
		{
		}
		void FeatureMatcher::execute() {
			const Mat& inputImage = getInputMat();
			setOutputMat(inputImage);

			trainDescriptors = queryDescriptors.clone();
			trainKeyPoints = queryKeyPoints;
			queryDescriptors = cv::Mat();
			queryKeyPoints.clear();

			FeatureMatcher::prepareAlgorithm();
			FeatureMatcher::featureDetector->detect(inputImage, queryKeyPoints);
			FeatureMatcher::extractor->compute(inputImage, queryKeyPoints, queryDescriptors);

			std::list<TrackingObject> trackingObjListByFeature;
			if(	queryDescriptors.cols == trainDescriptors.cols &&
				queryDescriptors.data != 0 && trainDescriptors.data != 0 &&
				queryKeyPoints.size() != 0 && trainKeyPoints.size() != 0)
			{
				matchFeatures(
					queryKeyPoints, queryDescriptors,
					trainKeyPoints, trainDescriptors, 
					dmatches);
			}
		}
		void FeatureMatcher::draw(cv::Mat& image, cv::Scalar& color) {
			TrackingObject::draw(image, dmatches, queryKeyPoints, trainKeyPoints, color);
		}

		void FeatureMatcher::prepareAlgorithm() {

			if(FeatureMatcher::featureDetector == 0) {
				//detector: FAST，FASTX，STAR，SIFT，SURF，ORB，BRISK，MSER，GFTT，HARRIS，Dense，SimpleBlob
				//descriptor: SIFT，SURF，BRIEF，BRISK，ORB，FREAK

				//fast. large radius. few miss onmatching
				FeatureMatcher::featureDetector = FeatureDetector::create("ORB");
				FeatureMatcher::extractor = cv::DescriptorExtractor::create("ORB");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming(2)");
				FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming");

				//fast. large radius. few miss onmatching
				//FeatureMatcher::featureDetector = FeatureDetector::create("ORB");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("FREAK");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming(2)");

				////much miss matching
				//FeatureMatcher::featureDetector = FeatureDetector::create("SimpleBlob");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("FREAK");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming(2)");

				////slow, much miss matching
				//FeatureMatcher::featureDetector = FeatureDetector::create("Dense");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("FREAK");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming(2)");

				////fast, much miss matching
				//FeatureMatcher::featureDetector = FeatureDetector::create("HARRIS");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("FREAK");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming(2)");

				////fast, much miss matching
				//FeatureMatcher::featureDetector = FeatureDetector::create("GFTT");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("FREAK");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming(2)");

				////slow, few features, much miss matching
				//FeatureMatcher::featureDetector = FeatureDetector::create("MSER");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("FREAK");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming(2)");

				////slow, few features, large radius, much miss matching
				//FeatureMatcher::featureDetector = FeatureDetector::create("MSER");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("ORB");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming(2)");

				////fast, simular SIFT/SURF, miss matching
				////速い。SIFT,SURFに似ている。マッチングで誤検出がすこしある。
				//FeatureMatcher::featureDetector = FeatureDetector::create("BRISK");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("BRISK");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming(2)");

				////fast, few miss matching, fixed radius of feature point
				////まあまあ速い。マッチングで誤検出が少ない。特徴点の半径が一定
				//FeatureMatcher::featureDetector = FeatureDetector::create("FASTX");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("SIFT");

				////slow feature detection, few miss matching
				////特徴点抽出が、すこし遅い。マッチングで誤検出が少ない
				//FeatureMatcher::featureDetector = FeatureDetector::create("FAST");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("SIFT");

				////fast, lot of miss matching
				////速い。マッチングで誤検出がある
				//FeatureMatcher::featureDetector = FeatureDetector::create("FASTX");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("SURF");

				////slow
				//FeatureMatcher::featureDetector = FeatureDetector::create("SIFT");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("SURF");

				////slow
				//FeatureMatcher::featureDetector = FeatureDetector::create("SIFT");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("SIFT");
				
				////slow, much feature point, radius large
				//FeatureMatcher::featureDetector = FeatureDetector::create("SURF");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("SURF");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("FlannBased");


				////slow, much feature point, radius large
				//FeatureMatcher::featureDetector = FeatureDetector::create("SURF");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("SIFT");

				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-L1");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming(2)");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("FlannBased");
			}
		}
		void FeatureMatcher::matchFeatures(
				std::vector<cv::KeyPoint>& queryKeyPoints, cv::Mat& queryDescriptors,
				std::vector<cv::KeyPoint>& trainKeyPoints, cv::Mat& trainDescriptors,
				std::vector<cv::DMatch>& dmatches)
		{
			dmatches.clear();

#ifdef MATCH_SIMGLE
			FeatureMatcher::matcher->match(queryDescriptors, trainDescriptors, dmatches);
#else
			std::vector<cv::DMatch> match12;
			std::vector<cv::DMatch> match21;
			FeatureMatcher::matcher->match(queryDescriptors, trainDescriptors, match12);
			FeatureMatcher::matcher->match(trainDescriptors, queryDescriptors, match21);
			for (size_t i = 0; i < match12.size(); i++) {
				cv::DMatch forward = match12[i];
				cv::DMatch backward = match21[forward.trainIdx];
				if (backward.trainIdx == forward.queryIdx) {
					dmatches.push_back(forward);
				}
			}

#endif

#ifndef NO_OMIT_ILLEGAL_MATCH
		std::vector<cv::DMatch> dmatches_all = dmatches;
		dmatches.clear();
		//measureDistance(dmatches_all, queryKeyPoints, trainKeyPoints);
		filterByDistanceStdDev(dmatches_all, 1.0, dmatches);
#endif
		}
		void FeatureMatcher::measureDistance(
			std::vector<cv::DMatch>& matches,
			const std::vector<cv::KeyPoint>& queryKeyPoints,
			const std::vector<cv::KeyPoint>& trainKeyPoints)
		{
			size_t src_count = matches.size();
			for (size_t i = 0; i < src_count; i++) {
				cv::Point2f p0 = queryKeyPoints[matches[i].queryIdx].pt;
				cv::Point2f p1 = trainKeyPoints[matches[i].trainIdx].pt;
				matches[i].distance = sqrt((p1.x - p0.x)*(p1.x - p0.x)+(p1.y - p0.y)*(p1.y - p0.y));
			}
		}
		void FeatureMatcher::filterByDistanceStdDev(
			const std::vector<cv::DMatch>& matches_src,
			float range_factor,
			std::vector<cv::DMatch>& dmatches)
		{
			float ave_distance = 0.0;
			size_t src_count = matches_src.size();
			for (size_t i = 0; i < src_count; i++) {
				ave_distance += matches_src[i].distance;
			}
			ave_distance /= src_count;

			float sdev_distance = 0.0;
			for (size_t i = 0; i < src_count; i++) {
				float diff_ave = matches_src[i].distance - ave_distance;
				sdev_distance += diff_ave * diff_ave;
			}
			sdev_distance = sqrt(sdev_distance / src_count);

			float distance_min = ave_distance - sdev_distance * range_factor;
			float distance_max = ave_distance + sdev_distance * range_factor;
			for (size_t i = 0; i < src_count; i++) {
				float distance = matches_src[i].distance;
				if(distance_min <= distance && distance <= distance_max) {
					dmatches.push_back(matches_src[i]);
				}
			}
		}
	}
}
