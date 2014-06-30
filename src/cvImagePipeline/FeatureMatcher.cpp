#include "FeatureMatcher.h"
//#define MATCH_SIMGLE
//#define NO_OMIT_ILLEGAL_MATCH
namespace cvUtils {
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

			//前回の画像から抽出した特徴点とマッチングする
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

			//画像全体の特徴点を抽出
			if(FeatureMatcher::featureDetector == 0) {
				//detector: FAST，FASTX，STAR，SIFT，SURF，ORB，BRISK，MSER，GFTT，HARRIS，Dense，SimpleBlob
				//descriptor: SIFT，SURF，BRIEF，BRISK，ORB，FREAK

				//速い。特徴点が同心円状に得られる。半径が大きい。マッチングで誤検出が少ない
				FeatureMatcher::featureDetector = FeatureDetector::create("ORB");
				FeatureMatcher::extractor = cv::DescriptorExtractor::create("ORB");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming(2)");
				FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming");

				////速い。特徴点が同心円状に得られる。マッチングで誤検出が少ない。FREAKが不要な特徴点を絞っている？
				//FeatureMatcher::featureDetector = FeatureDetector::create("ORB");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("FREAK");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming(2)");

				////マッチング誤検出多い。
				//FeatureMatcher::featureDetector = FeatureDetector::create("SimpleBlob");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("FREAK");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming(2)");

				////超遅い、マッチング誤検出多い。密度が高い。
				//FeatureMatcher::featureDetector = FeatureDetector::create("Dense");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("FREAK");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming(2)");

				////速いがマッチングで誤検出多い
				//FeatureMatcher::featureDetector = FeatureDetector::create("HARRIS");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("FREAK");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming(2)");

				////速いが。マッチングで誤検出多い
				//FeatureMatcher::featureDetector = FeatureDetector::create("GFTT");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("FREAK");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming(2)");

				////遅い。特徴点が少ない。マッチングで誤検出がある。
				//FeatureMatcher::featureDetector = FeatureDetector::create("MSER");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("FREAK");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming(2)");

				////遅い。特徴点が少ない。半径が大きいマッチングで誤検出がある。
				//FeatureMatcher::featureDetector = FeatureDetector::create("MSER");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("ORB");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming(2)");

				////速い。SIFT,SURFに似ている。マッチングで誤検出がすこしある。
				//FeatureMatcher::featureDetector = FeatureDetector::create("BRISK");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("BRISK");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming(2)");

				////まあまあ速い。マッチングで誤検出が少ない。特徴点の半径が一定
				//FeatureMatcher::featureDetector = FeatureDetector::create("FASTX");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("SIFT");

				////特徴点抽出が、すこし遅い。マッチングで誤検出が少ない
				//FeatureMatcher::featureDetector = FeatureDetector::create("FAST");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("SIFT");

				////速い。マッチングで誤検出がある
				//FeatureMatcher::featureDetector = FeatureDetector::create("FASTX");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("SURF");

				////すこし遅い
				//FeatureMatcher::featureDetector = FeatureDetector::create("SIFT");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("SURF");

				////SIFT/SIFT 半径が大きいのから小さいのまで。少し遅い
				//FeatureMatcher::featureDetector = FeatureDetector::create("SIFT");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("SIFT");
				
				////SURF/SURF 大量の特徴点 半径が総じて大きい。少し遅い
				//FeatureMatcher::featureDetector = FeatureDetector::create("SURF");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("SURF");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("FlannBased");


				////大量の特徴点、半径が大きい、遅い。使えない
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
			//誤判定が多いので双方向のチェックを行う
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
		//マッチした特徴点間の距離の異常値を排除する。
		//（元のdistanceではうまく排除できないため距離を計算する）
		std::vector<cv::DMatch> dmatches_all = dmatches;
		dmatches.clear();
		//measureDistance(dmatches_all, queryKeyPoints, trainKeyPoints);
		filterByDistanceStdDev(dmatches_all, 1.0, dmatches);
#endif
		}
		//DMatch::distanceの値が信用ならないので独自に計算する。
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
			//マッチした特徴点同士の距離とその平均値を求める
			float ave_distance = 0.0;
			size_t src_count = matches_src.size();
			for (size_t i = 0; i < src_count; i++) {
				ave_distance += matches_src[i].distance;
			}
			ave_distance /= src_count;

			//標準偏差を求める
			float sdev_distance = 0.0;
			for (size_t i = 0; i < src_count; i++) {
				float diff_ave = matches_src[i].distance - ave_distance;
				sdev_distance += diff_ave * diff_ave;
			}
			sdev_distance = sqrt(sdev_distance / src_count);

			//異常値を排除する。
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
