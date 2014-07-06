#include "FlowTracker.h"
#include <vector>
#include <sstream>
namespace cvImagePipeline {
	namespace Filter {
		using namespace std;
		using namespace cv;
		IMPLEMENT_CVFILTER(FlowTracker);
		FlowTracker::FlowTracker()
			: labeling_vec_thrs("labeling_vec_thrs", (float)0.3)
		{
			defInputMat("drawbase");
			defParam(labeling_vec_thrs);
			defParam(labeler.labeling_region_min_size);
		}

		FlowTracker::~FlowTracker() {
		}

		void FlowTracker::execute() {
			const Mat& flow = getInputMat();
			const Mat& originalImage = getInputMat("drawbase");
			labeler_buffer.setFlow(flow, labeling_vec_thrs);
			labeler.setBuffer(&labeler_buffer);
			labeler.setInputMat(flow);
			labeler.execute();

			//オプティカルフローの結果からトラッキング対象のオブジェクトを生成する。
			std::list<LabeledObject> labeled_obj_list = labeler.getLabeledObjects();
			LabeledObject::chooseByArea(labeled_obj_list, cv::Size(flow.cols, flow.rows).area() * 0.3);
			std::list<TrackingObject> trackingOjectListByFlow;
			TrackingObject::createTrackingObjects(labeled_obj_list, trackingOjectListByFlow);

			int nDetectNew = trackingOjectListByFlow.size();

			std::vector<DMatch> matches;
			FlowTracker::createMatchScoreArray(matches, tracking_object_list, trackingOjectListByFlow);
			TrackingObject::eliminateExtraMatches(matches, tracking_object_list, trackingOjectListByFlow);
			TrackingObject::updateTrackingObjects(tracking_object_list, trackingOjectListByFlow, matches);

			refOutputMat() = originalImage.clone();
			TrackingObject::draw(refOutputMat(), tracking_object_list);

		}
		void FlowTracker::Buffer::drawFlow(const Mat& flow, Mat& image, double labeling_vec_thrs) {
			int src_ch = flow.channels();
			int dst_ch = image.channels();
			for(int y = 0; y < flow.rows; y++) {
				float* src = (float*)(flow.data + y * flow.step);
				unsigned char* dst = image.data + y * image.step;
				for(int x = 0; x < flow.cols; x++) {
					float value = src[0] * src[0] + src[1] * src[1];
					if(value >= labeling_vec_thrs) {
						*dst = 255;
					} else {
						*dst = 0;
					}
					src += src_ch;
					dst += dst_ch;
				}
			}
		}
		void FlowTracker::Buffer::setFlow(const Mat& flow, double labeling_vec_thrs) {
			Mat featImg(Mat::zeros(flow.rows, flow.cols, CV_8UC3));
			drawFlow(flow, featImg, labeling_vec_thrs);
			setLabelingSourceBuffer(featImg);
		}
		void FlowTracker::createMatchScoreArray(
			std::vector<DMatch>& matches,
			const std::list<TrackingObject>& tracking_object_list,
			const std::list<TrackingObject>& trackingOjectListByFlow)
		{
			//新オブジェクトと旧オブジェクトのマッチスコアを計算
			std::list<TrackingObject>::const_iterator trainObj = tracking_object_list.begin();
			for(int trainIdx = 0; trainObj != tracking_object_list.end(); trainObj++, trainIdx++) {
				std::list<TrackingObject>::const_iterator queryObj = trackingOjectListByFlow.begin();
				for(int queryIdx = 0; queryObj != trackingOjectListByFlow.end(); queryObj++, queryIdx++) {
					float score = TrackingObject::calcMatchStatus(*queryObj, *trainObj);
					matches.push_back(DMatch(queryIdx, trainIdx, score));
				}
			}
		}
	}
}
