#pragma once
#include <opencv2/opencv.hpp>
#include "Labeling.h"
namespace cvUtils {
	#pragma warning(disable:4251)
	struct __declspec(dllexport) LabeledObject {
	public:
		cv::Rect region;
		//int left;	//�̈捶�[
		//int top;	//�̈��[
		//int right;	//�̈�E�[
		//int bottom;	//�̈扺�[
		float avg_vx;	//�̈���ړ��x�N�g��X����
		float avg_vy;	//�̈���ړ��x�N�g��Y����
		long clock;
	public:
		LabeledObject();
		bool operator==(const LabeledObject& that) const;
		cv::Size size() const { return region.size(); }
		cv::Point center() const { return cv::Point( region.x + region.width / 2, region.y + region.height /2); }
		const cv::Rect& rect() const { return region; }

		bool contains(const cv::Point& pt) const { return region.contains(pt); }
		void putTo(std::ostream& os) const;
	public:
		static void chooseByArea(std::list<LabeledObject>& obj_list, double max_area_size);
	};
	#pragma warning(default:4251)
}
__declspec(dllexport) std::ostream& operator << (std::ostream& os, const cvUtils::LabeledObject& labeledObject);
