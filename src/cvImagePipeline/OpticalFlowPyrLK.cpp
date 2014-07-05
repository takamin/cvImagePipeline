#include <windows.h>
#include "OpticalFlowPyrLK.h"

namespace cvImagePipeline {
	namespace Filter {

		using namespace std;
		using namespace cv;

		IMPLEMENT_CVFILTER(OpticalFlowPyrLK);
		OpticalFlowPyrLK::OpticalFlowPyrLK() { }
		void OpticalFlowPyrLK::reset() 
		{
			prevMat = Mat();
		}
		void OpticalFlowPyrLK::execute() {
			const Mat& inputMat = getInputMat();
			if(!prevMat.empty()) {
				std::vector<cv::KeyPoint> keypoints;
				std::vector<cv::Point2f> points;
				cv::GoodFeaturesToTrackDetector detector(100, 0.05, 3);
				detector.detect(prevMat, keypoints);
				std::vector<cv::KeyPoint>::iterator kp = keypoints.begin();
				for(;kp != keypoints.end(); ++kp) {
					points.push_back(kp->pt);
				}
				std::vector<cv::Point2f> newpoints;
				std::vector<unsigned char> statuses;
				std::vector<float> errors;
				cv::calcOpticalFlowPyrLK(prevMat, inputMat, points, newpoints,
					statuses, errors, cv::Size(21,21), 3,
					cvTermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.05), 0);
				Mat& outputMat = refOutputMat();
				outputMat = inputMat.clone();
				std::vector<cv::Point2f>::iterator prev_point = points.begin();
				std::vector<cv::Point2f>::iterator new_point = newpoints.begin();
				std::vector<unsigned char>::iterator status = statuses.begin();
				while(prev_point != points.end()) {	
					if(*status != 0) {
						cv::line(outputMat, *prev_point, *new_point, cv::Scalar(0,0,0), 1);
					}
					cv::circle(outputMat, *new_point, 3, cv::Scalar(0,0,0));
					prev_point++;
					new_point++;
				}
			}
			prevMat = inputMat.clone();
		}
	}
}
