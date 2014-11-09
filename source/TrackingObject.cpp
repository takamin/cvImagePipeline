#include "TrackingObject.h"

namespace cvImagePipeline {
	using namespace std;
	using namespace cv;

	TrackingObject::TrackingObject(const LabeledObject& labeledObject)
	: state(labeledObject), life(0), stopCount(0), end(false)
	{
	}
	bool TrackingObject::operator==(const TrackingObject& that)
	{
		return this->state == that.state;
	}

	void TrackingObject::update(TrackingObject newItem)
	{
		history.push_back(state);
		if(history.size() > 300) {
			history.remove(*history.rbegin());
		}
		cv::Point p0min(state.region.tl());
		cv::Point p0max(state.region.br());
		cv::Point p1min(newItem.state.region.tl());
		cv::Point p1max(newItem.state.region.br());
		newItem.state.avg_vx = (((float)p1min.x - p0min.x) + (p1max.x - p0max.x)) / 2;
		newItem.state.avg_vy = (((float)p1min.y - p0min.y) + (p1max.y - p0max.y)) / 2;
		state = newItem.state;
		life++;
		updated = true;
		dmatches = newItem.dmatches;
		stopCount = 0;
		end = false;
	}
	void TrackingObject::noupdate() {
		//LabeledObject labeledObject;
		//labeledObject.avg_vx = 0;
		//labeledObject.avg_vy = 0;
		//labeledObject.region = state.region;
		//history.push_back(state);
		//if(history.size() > 300) {
		//	history.remove(*history.rbegin());
		//}
		//state = labeledObject;
		life++;
		dmatches.clear();
		stopCount++;
		if(stopCount > 10) {
			end = true;
		}
	}

	void TrackingObject::putTo(std::ostream& os) const
	{
		os << "life:" << life << ",";
		os << "rect:" << state.region;
	}

	void TrackingObject::addMatch(const cv::DMatch& dmatch) {
		dmatches.push_back(dmatch);
	}
	
	void TrackingObject::chooseMatches(
		const std::vector<cv::DMatch>& dmatches,
		const std::vector<cv::KeyPoint>& queryKeyPoints,
		const std::vector<cv::KeyPoint>& trainKeyPoints,
		std::vector<cv::DMatch>& matchesChose) const
	{
		std::vector<cv::DMatch>::const_iterator prev_dmatch = this->dmatches.begin();
		while(prev_dmatch != this->dmatches.end()) {
			std::vector<cv::DMatch>::const_iterator dmatch = dmatches.begin();
			while(dmatch != dmatches.end()) {
				if(prev_dmatch->queryIdx == dmatch->trainIdx) {
//					if(state.isInRect(queryKeyPoints[dmatch->queryIdx].pt)) {
						matchesChose.push_back(*dmatch);
//					}
				}
				dmatch++;
			}
			prev_dmatch++;
		}
	}

	TrackingObject TrackingObject::createByFeature(
		const std::vector<cv::DMatch>& dmatches,
		const std::vector<cv::KeyPoint>& queryKeyPoints,
		const std::vector<cv::KeyPoint>& trainKeyPoints) const
	{
		std::vector<cv::DMatch>::const_iterator dmatch = dmatches.begin();
		while(dmatch != dmatches.end()) {
			const KeyPoint& queryKeyPoint = queryKeyPoints[dmatch->queryIdx];
			const KeyPoint& trainKeyPoint = trainKeyPoints[dmatch->trainIdx];
			dmatch++;
		}

		LabeledObject object = state;
		TrackingObject newObject(object);

		dmatch = dmatches.begin();
		while(dmatch != dmatches.end()) {
			newObject.addMatch(*dmatch);
			dmatch++;
		}
		return newObject;
	}
	void TrackingObject::createTrackingObjects(
		const std::list<LabeledObject>& labeled_object_list,
		std::list<TrackingObject>& tracking_obj)
	{
		for(list<LabeledObject>::const_iterator labeledObject = labeled_object_list.begin();
			labeledObject != labeled_object_list.end(); labeledObject++)
		{
			tracking_obj.push_back(TrackingObject(*labeledObject));
		}
	}
	void TrackingObject::setFeatures(
		std::list<TrackingObject>& trackingOjectListByFlow,
		const std::vector<DMatch>& dmatches,
		const std::vector<KeyPoint>& queryKeyPoints)
	{
		std::list<TrackingObject>::iterator trackingObj = trackingOjectListByFlow.begin();
		while(trackingObj != trackingOjectListByFlow.end()) {
			std::vector<DMatch>::const_iterator dmatch = dmatches.begin();
			int matchIndex = 0;
			while(dmatch != dmatches.end()) {
				if(trackingObj->state.contains(queryKeyPoints[dmatch->queryIdx].pt)) {
					DMatch m(*dmatch);
					trackingObj->addMatch(m);
				}
				matchIndex++;
				dmatch++;
			}
			trackingObj++;
		}
	}
	float TrackingObject::calcMatchStatus(
		const TrackingObject& newObj,
		const TrackingObject& oldObj)
	{
		cv::Point2f center0 = oldObj.state.center();
		center0.x += oldObj.state.avg_vx / 2;
		center0.y += oldObj.state.avg_vy / 2;
		cv::Point2f center1 = newObj.state.center();
		cv::Size size0 = oldObj.state.size();
		cv::Size size1 = newObj.state.size();
		cv::Point2f d = center1 - center0;
		float center_distance = sqrt(d.x * d.x + d.y * d.y);
		float dist_status = (float)(1.0 / sqrt((-1.0 - center_distance) * (-1.0 - center_distance)));

		float area0 = (float)size0.area();
		float area1 = (float)size1.area();
		float area_status = 0.0;
		if(area0 > area1) {
			area_status = area1 / area0;
		} else {
			area_status = area0 / area1;
		}
		float asp1 = (float)size1.width / size1.height;
		float asp0 = (float)size0.width / size0.height;
		float asp_status = 0.0;
		if(asp0 > asp1) {
			asp_status = asp1 / asp0;
		} else {
			asp_status = asp0 / asp1;
		}
		//return dist_status + area_status + asp_status;
		//return 3.25 * dist_status + area_status + asp_status;
		return (float)((3.25 * dist_status + area_status + 5.0 * asp_status) * ((20 - oldObj.stopCount) / 20 * 0.5));
		//return (float)(3.25 * dist_status + area_status + 0 * asp_status) * (oldObj.end? 0.5 : 1.0);
	}
	float TrackingObject::calcMatchStatus(
		const TrackingObject& newObj,
		const TrackingObject& oldObj,
		const vector<KeyPoint>& queryKeyPoints,
		const vector<KeyPoint>& trainKeyPoints)
	{
		vector<DMatch>::const_iterator dmatch = newObj.dmatches.begin();
		int match_count = 0;
		int total_count = 0;
		while(dmatch != newObj.dmatches.end()) {
			const KeyPoint& queryKeyPoint = queryKeyPoints[dmatch->queryIdx];
			const KeyPoint& trainKeyPoint = trainKeyPoints[dmatch->trainIdx];
			if(newObj.state.contains(queryKeyPoint.pt)) {
				total_count++;
				if(oldObj.state.contains(trainKeyPoint.pt)) {
					match_count++;
				}
			}
			dmatch++;
		}
		float match_ratio = (float)match_count / total_count;
		return match_ratio;

	}
	void TrackingObject::eliminateExtraMatches(
		std::vector<cv::DMatch> matches,
		const std::list<TrackingObject>& tracking_object_list,
		const std::list<TrackingObject>& trackingOjectListByFlow)
	{
		std::vector<DMatch>::iterator match0 = matches.begin();
		for(; match0 != matches.end(); match0++) {
			std::vector<DMatch>::iterator match1 = match0;
			match1++;
			for(; match1 != matches.end(); match1++) {
				if(match1->distance > match0->distance) {
					DMatch tmp = *match0;
						*match0 = *match1;
					*match1 = tmp;
				}
			}
		}
		int train_object_count = tracking_object_list.size();
		for(int trainIdx = 0; trainIdx < train_object_count; trainIdx++) {
			bool found = false;
			std::vector<DMatch>::iterator match = matches.begin();
			while(match != matches.end()) {
				if(trainIdx == match->trainIdx) {
					if(found) {
						match = matches.erase(match);
						continue;
					} else {
						found = true;
					}
				}
				match++;
			}
		}

		int query_object_count = trackingOjectListByFlow.size();
		for(int queryIdx = 0; queryIdx < query_object_count; queryIdx++) {
			bool found = false;
			std::vector<DMatch>::iterator match = matches.begin();
			while(match != matches.end()) {
				if(queryIdx == match->queryIdx) {
					if(found) {
						match = matches.erase(match);
						continue;
					} else {
						found = true;
					}
				}
				match++;
			}
		}
	}
	void TrackingObject::updateTrackingObjects(
		std::list<TrackingObject>& tracking_object_list,
		std::list<TrackingObject>& trackingOjectListByFlow,
		const std::vector<cv::DMatch>& matches)
	{
		std::list<TrackingObject>::iterator trainObj = tracking_object_list.begin();
		for(int trainIdx = 0; trainObj != tracking_object_list.end(); trainObj++, trainIdx++) {
			bool updated = false;
			std::list<TrackingObject>::iterator queryObj = trackingOjectListByFlow.begin();
			for(int queryIdx = 0; queryObj != trackingOjectListByFlow.end(); queryObj++, queryIdx++) {
				std::vector<DMatch>::const_iterator match = matches.begin();
				while(match != matches.end()) {
					if(queryIdx == match->queryIdx && trainIdx == match->trainIdx) {
						trainObj->update(*queryObj);
						trackingOjectListByFlow.remove(*queryObj);
						updated = true;
						break;
					}
					match++;
				}
				if(updated) {
					break;
				}
			}
			if(!updated) {
				trainObj->noupdate();
			}
		}
		std::list<TrackingObject>::iterator itnew = trackingOjectListByFlow.begin();
		//bool add = false;
		while(itnew != trackingOjectListByFlow.end()) {
			tracking_object_list.push_back(*itnew);
			itnew++;
			//add = true;
		}
	}
	void TrackingObject::eraceOutMatches(
		std::list<TrackingObject>& tracking_object_list,
		const std::vector<KeyPoint>& queryKeyPoints,
		const std::vector<KeyPoint>& trainKeyPoints)
	{
		for(list<TrackingObject>::iterator tracking_object = tracking_object_list.begin();
			tracking_object != tracking_object_list.end(); tracking_object++)
		{
			std::vector<DMatch>::iterator dmatch = tracking_object->dmatches.begin();
			while(dmatch != tracking_object->dmatches.end()) {
				if(!tracking_object->state.region.contains(queryKeyPoints[dmatch->queryIdx].pt)) {
					//cv::line(refOutputMat(),
					//	queryKeyPoints[dmatch->queryIdx].pt,
					//	trainKeyPoints[dmatch->trainIdx].pt, cv::Scalar(0,0,255), 1);
					dmatch = tracking_object->dmatches.erase(dmatch);
				} else {
					dmatch++;
				}
			}
		}
	}
	void TrackingObject::draw(Mat& outputMat, const std::list<TrackingObject>&  tracking_object_list) {
		static cv::Scalar label_colors32f[] = {
			cv::Scalar(0.0,		1.0,	0.0	),
			cv::Scalar(0.0,		0.0,	1.0	),
			cv::Scalar(0.0,		1.0,	1.0	),
			cv::Scalar(1.0,		0.0,	0.0	),
			cv::Scalar(1.0,		1.0,	0.0	),
			cv::Scalar(0.0,		1.0,	1.0	),
			cv::Scalar(1.0,		1.0,	1.0	),
		};
		static cv::Scalar label_colors8u[] = {
			cv::Scalar(0.0,		255,	0.0	),
			cv::Scalar(0.0,		0.0,	255	),
			cv::Scalar(0.0,		255,	255	),
			cv::Scalar(255,		0.0,	0.0	),
			cv::Scalar(255,		255,	0.0	),
			cv::Scalar(0.0,		255,	255	),
			cv::Scalar(255,		255,	255	),
		};
		cv::Scalar* label_colors = ((outputMat.type() & CV_32F) != 0) ? label_colors32f : label_colors8u;
		std::list<TrackingObject>::const_iterator it = tracking_object_list.begin();
		int objIndex = 0;
		while(it != tracking_object_list.end()) {
			cv::Scalar& color = label_colors[objIndex % 8];
			if(!it->end) {
				it->draw(outputMat, color);
				std::stringstream ss;
				ss << "#" << objIndex;
				cv::putText(
					outputMat, ss.str(), cv::Point(it->state.region.x,
					it->state.region.y + it->state.region.height / 2),
					FONT_HERSHEY_SIMPLEX, 0.3, color);

			}
			objIndex++;
			it++;
		}
	}
	void TrackingObject::draw(Mat& outputMat,
		const std::list<TrackingObject>&  tracking_object_list,
		const std::vector<KeyPoint>& queryKeyPoints,
		const std::vector<KeyPoint>& trainKeyPoints)
	{
		static cv::Scalar label_colors32f[] = {
			cv::Scalar(0.0,		1.0,	0.0	),
			cv::Scalar(0.0,		0.0,	1.0	),
			cv::Scalar(0.0,		1.0,	1.0	),
			cv::Scalar(1.0,		0.0,	0.0	),
			cv::Scalar(1.0,		1.0,	0.0	),
			cv::Scalar(0.0,		1.0,	1.0	),
			cv::Scalar(1.0,		1.0,	1.0	),
		};
		static cv::Scalar label_colors8u[] = {
			cv::Scalar(0.0,		255,	0.0	),
			cv::Scalar(0.0,		0.0,	255	),
			cv::Scalar(0.0,		255,	255	),
			cv::Scalar(255,		0.0,	0.0	),
			cv::Scalar(255,		255,	0.0	),
			cv::Scalar(0.0,		255,	255	),
			cv::Scalar(255,		255,	255	),
		};
		cv::Scalar* label_colors = ((outputMat.type() & CV_32F) != 0) ? label_colors32f : label_colors8u;
		std::list<TrackingObject>::const_iterator it = tracking_object_list.begin();
		int objIndex = 0;
		while(it != tracking_object_list.end()) {
			cv::Scalar& color = label_colors[objIndex % 8];
			if(!it->end) {
				it->draw(outputMat, queryKeyPoints, trainKeyPoints, color);
				std::stringstream ss;
				ss << "#" << objIndex;
				cv::putText(
					outputMat, ss.str(), cv::Point(it->state.region.x,
					it->state.region.y + it->state.region.height / 2),
					FONT_HERSHEY_SIMPLEX, 0.3, color);

			}
			objIndex++;
			it++;
		}
	}
	void TrackingObject::draw(cv::Mat& image,
		const std::vector<cv::DMatch> dmatches,
		const vector<KeyPoint>& queryKeyPoint,
		const vector<KeyPoint>& trainKeyPoint,
		const cv::Scalar& color)
	{
		vector<DMatch>::const_iterator dmatch = dmatches.begin();
		while(dmatch != dmatches.end()) {
			const KeyPoint& kp = queryKeyPoint[dmatch->queryIdx];
			cv::circle(image, kp.pt, 3/*(int)kp.size*/, color, 1);
			cv::line(image, kp.pt, trainKeyPoint[dmatch->trainIdx].pt, color, 1);
			dmatch++;
		}
	}

	void TrackingObject::draw(cv::Mat& image,
		const vector<KeyPoint>& queryKeyPoint,
		const vector<KeyPoint>& trainKeyPoint,
		const cv::Scalar& color) const
	{
		draw(image, color);
		TrackingObject::draw(image, dmatches, queryKeyPoint, trainKeyPoint, color);
	}
	void TrackingObject::draw(cv::Mat& image, const cv::Scalar& color) const
	{
		cv::rectangle(image, state.region, color);
	}
}
std::ostream& operator << (std::ostream& ostr, const cvImagePipeline::TrackingObject& trackingObject) {
	trackingObject.putTo(ostr);
	//ostr << "{life:" << trackingObject.life << ",";
	//ostr << "rect:(";
	//ostr << trackingObject.state.left << ",";
	//ostr << trackingObject.state.top << ")-(";
	//ostr << trackingObject.state.right << ",";
	//ostr << trackingObject.state.bottom << ")}";
	return ostr;
}

