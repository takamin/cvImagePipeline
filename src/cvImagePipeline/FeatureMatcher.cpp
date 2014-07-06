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

			//�O��̉摜���璊�o���������_�ƃ}�b�`���O����
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

			//�摜�S�̂̓����_�𒊏o
			if(FeatureMatcher::featureDetector == 0) {
				//detector: FAST�CFASTX�CSTAR�CSIFT�CSURF�CORB�CBRISK�CMSER�CGFTT�CHARRIS�CDense�CSimpleBlob
				//descriptor: SIFT�CSURF�CBRIEF�CBRISK�CORB�CFREAK

				//�����B�����_�����S�~��ɓ�����B���a���傫���B�}�b�`���O�Ō댟�o�����Ȃ�
				FeatureMatcher::featureDetector = FeatureDetector::create("ORB");
				FeatureMatcher::extractor = cv::DescriptorExtractor::create("ORB");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming(2)");
				FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming");

				////�����B�����_�����S�~��ɓ�����B�}�b�`���O�Ō댟�o�����Ȃ��BFREAK���s�v�ȓ����_���i���Ă���H
				//FeatureMatcher::featureDetector = FeatureDetector::create("ORB");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("FREAK");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming(2)");

				////�}�b�`���O�댟�o�����B
				//FeatureMatcher::featureDetector = FeatureDetector::create("SimpleBlob");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("FREAK");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming(2)");

				////���x���A�}�b�`���O�댟�o�����B���x�������B
				//FeatureMatcher::featureDetector = FeatureDetector::create("Dense");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("FREAK");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming(2)");

				////�������}�b�`���O�Ō댟�o����
				//FeatureMatcher::featureDetector = FeatureDetector::create("HARRIS");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("FREAK");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming(2)");

				////�������B�}�b�`���O�Ō댟�o����
				//FeatureMatcher::featureDetector = FeatureDetector::create("GFTT");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("FREAK");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming(2)");

				////�x���B�����_�����Ȃ��B�}�b�`���O�Ō댟�o������B
				//FeatureMatcher::featureDetector = FeatureDetector::create("MSER");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("FREAK");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming(2)");

				////�x���B�����_�����Ȃ��B���a���傫���}�b�`���O�Ō댟�o������B
				//FeatureMatcher::featureDetector = FeatureDetector::create("MSER");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("ORB");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming(2)");

				////�����BSIFT,SURF�Ɏ��Ă���B�}�b�`���O�Ō댟�o������������B
				//FeatureMatcher::featureDetector = FeatureDetector::create("BRISK");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("BRISK");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("BruteForce-Hamming(2)");

				////�܂��܂������B�}�b�`���O�Ō댟�o�����Ȃ��B�����_�̔��a�����
				//FeatureMatcher::featureDetector = FeatureDetector::create("FASTX");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("SIFT");

				////�����_���o���A�������x���B�}�b�`���O�Ō댟�o�����Ȃ�
				//FeatureMatcher::featureDetector = FeatureDetector::create("FAST");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("SIFT");

				////�����B�}�b�`���O�Ō댟�o������
				//FeatureMatcher::featureDetector = FeatureDetector::create("FASTX");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("SURF");

				////�������x��
				//FeatureMatcher::featureDetector = FeatureDetector::create("SIFT");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("SURF");

				////SIFT/SIFT ���a���傫���̂��珬�����̂܂ŁB�����x��
				//FeatureMatcher::featureDetector = FeatureDetector::create("SIFT");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("SIFT");
				
				////SURF/SURF ��ʂ̓����_ ���a�������đ傫���B�����x��
				//FeatureMatcher::featureDetector = FeatureDetector::create("SURF");
				//FeatureMatcher::extractor = cv::DescriptorExtractor::create("SURF");
				//FeatureMatcher::matcher = cv::DescriptorMatcher::create("FlannBased");


				////��ʂ̓����_�A���a���傫���A�x���B�g���Ȃ�
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
			//�딻�肪�����̂őo�����̃`�F�b�N���s��
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
		//�}�b�`���������_�Ԃ̋����ُ̈�l��r������B
		//�i����distance�ł͂��܂��r���ł��Ȃ����ߋ������v�Z����j
		std::vector<cv::DMatch> dmatches_all = dmatches;
		dmatches.clear();
		//measureDistance(dmatches_all, queryKeyPoints, trainKeyPoints);
		filterByDistanceStdDev(dmatches_all, 1.0, dmatches);
#endif
		}
		//DMatch::distance�̒l���M�p�Ȃ�Ȃ��̂œƎ��Ɍv�Z����B
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
			//�}�b�`���������_���m�̋����Ƃ��̕��ϒl�����߂�
			float ave_distance = 0.0;
			size_t src_count = matches_src.size();
			for (size_t i = 0; i < src_count; i++) {
				ave_distance += matches_src[i].distance;
			}
			ave_distance /= src_count;

			//�W���΍������߂�
			float sdev_distance = 0.0;
			for (size_t i = 0; i < src_count; i++) {
				float diff_ave = matches_src[i].distance - ave_distance;
				sdev_distance += diff_ave * diff_ave;
			}
			sdev_distance = sqrt(sdev_distance / src_count);

			//�ُ�l��r������B
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
