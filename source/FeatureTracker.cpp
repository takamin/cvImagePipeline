#include "FeatureTracker.h"
#include "FlowTracker.h"
#include "FeatureMatcher.h"

#include <vector>
#include <sstream>
namespace cvImagePipeline {
	namespace Filter {

		using namespace std;
		using namespace cv;

		IMPLEMENT_CVFILTER(FeatureTracker);
		FeatureTracker::FeatureTracker()
		{
			defInputMat("drawbase");
			defParam(labeler.labeling_region_min_size);
		}
		FeatureTracker::~FeatureTracker() {
		}
		void FeatureTracker::execute() {
			const Mat& originalImage = getInputMat();
			featureMatcher.setInputMat(originalImage);
			featureMatcher.execute();
			vector<DMatch> dmatches = featureMatcher.getMatches();
			const vector<KeyPoint>& queryKeyPoints = featureMatcher.getQueryKeyPoints();
			const vector<KeyPoint>& trainKeyPoints = featureMatcher.getTrainKeyPoints();

			/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
			std::list<TrackingObject>::iterator tracking_object = tracking_object_list.begin();
			std::list<TrackingObject> feature_match_object;
			while(tracking_object != tracking_object_list.end()) {
				std::vector<DMatch>::const_iterator train_match = tracking_object->dmatches.begin();
				std::vector<DMatch> new_matches;
				bool match_at_least_one = false;
				Point pt,tl(0,0),br(0,0);
				while(train_match != tracking_object->dmatches.end()) {
					std::vector<DMatch>::iterator query_match = dmatches.begin();
					while(query_match != dmatches.end()) {
						if(query_match->trainIdx == train_match->queryIdx) {
							new_matches.push_back(*query_match);
							pt = queryKeyPoints[query_match->queryIdx].pt;
							if(!match_at_least_one) {
								tl = br = pt;
								match_at_least_one = true;
							} else {
								if(tl.x > pt.x) { tl.x = pt.x; }
								if(br.x < pt.x) { br.x = pt.x; }
								if(tl.y > pt.y) { tl.y = pt.y; }
								if(br.y < pt.y) { br.y = pt.y; }
							}
							query_match = dmatches.erase(query_match);
						} else {
							query_match++;
						}
					}
					train_match++;
				}
				Rect newRegion(tl, br);
				if(match_at_least_one && newRegion.area() > 100) {
					LabeledObject labeled_object;
					labeled_object.region = Rect(tl, br);
					TrackingObject new_state(labeled_object);
					new_state.dmatches = new_matches;
					tracking_object->update(new_state);
					feature_match_object.push_back(*tracking_object);
					tracking_object = tracking_object_list.erase(tracking_object);
				} else {
					tracking_object++;
					for(vector<DMatch>::iterator match = new_matches.begin();
						match != new_matches.end(); match++)
					{
						dmatches.push_back(*match);
					}
				}
			}
			/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/

			labeler_buffer.setFeature(
				originalImage.cols, originalImage.rows,
				dmatches, queryKeyPoints, trainKeyPoints);
			labeler.setBuffer(&labeler_buffer);
			labeler.setInputMat(originalImage);
			labeler.execute();

			std::list<LabeledObject> labeled_obj_list = labeler.getLabeledObjects();
			LabeledObject::chooseByArea(labeled_obj_list, cv::Size(originalImage.cols,
				originalImage.rows).area() * 0.3);
			std::list<TrackingObject> new_tracking_object_list;
			TrackingObject::createTrackingObjects(labeled_obj_list, new_tracking_object_list);

			TrackingObject::setFeatures(new_tracking_object_list, dmatches, queryKeyPoints);

			std::vector<DMatch> match_scores;
			std::list<TrackingObject>::const_iterator trainObj = tracking_object_list.begin();
			for(int trainIdx = 0; trainObj != tracking_object_list.end(); trainObj++, trainIdx++) {
				std::list<TrackingObject>::const_iterator queryObj = new_tracking_object_list.begin();
				for(int queryIdx = 0; queryObj != new_tracking_object_list.end(); queryObj++, queryIdx++) {
					float score = (float)(TrackingObject::calcMatchStatus(
						*queryObj, *trainObj, queryKeyPoints, trainKeyPoints));
					match_scores.push_back(DMatch(queryIdx, trainIdx, score));
				}
			}
			TrackingObject::eliminateExtraMatches(match_scores, tracking_object_list, new_tracking_object_list);
			TrackingObject::updateTrackingObjects(tracking_object_list, new_tracking_object_list, match_scores);
			TrackingObject::eraceOutMatches(tracking_object_list, queryKeyPoints, trainKeyPoints);

			/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
			for(list<TrackingObject>::iterator featMatchObj = feature_match_object.begin();
				featMatchObj != feature_match_object.end(); featMatchObj++)
			{
				tracking_object_list.push_back(*featMatchObj);
			}
			list<TrackingObject>::iterator t0 = tracking_object_list.begin();
			while(t0 != tracking_object_list.end()) {
				list<TrackingObject>::iterator t1 = t0;
				t1++;
				while(t1 != tracking_object_list.end()) {
					if(t0->state.region.contains(t1->state.region.tl())
						&& t0->state.region.contains(t1->state.region.br()))
					{
						t1 = tracking_object_list.erase(t1);
					} else {
						t1++;
					}
				}
				t0++;
			}
			/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
			refOutputMat() = getInputMat("drawbase").clone();
			TrackingObject::draw(refOutputMat(), tracking_object_list, queryKeyPoints, trainKeyPoints);
		}
		void FeatureTracker::Buffer::setFeature(
			int cols, int rows,
			const std::vector<cv::DMatch>& matches,
			const vector<KeyPoint>& queryKeyPoints,
			const vector<KeyPoint>& trainKeyPoints)
		{
			Mat featImg(Mat::zeros(rows, cols, CV_8UC3));
			std::vector<cv::DMatch>::const_iterator match = matches.begin();
			while(match != matches.end()) {
				cv::circle(featImg, queryKeyPoints[match->queryIdx].pt, 15, cv::Scalar(128,0,0), 2);
				match++;
			}
			setLabelingSourceBuffer(featImg);
		}
	}
}
