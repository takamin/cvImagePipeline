#pragma once
#include <opencv2/opencv.hpp>
#include "Labeling.h"
#include "ImageProcessor.h"
namespace cvImagePipeline {
	#pragma warning(disable:4251)
	struct SHARED LabeledObject {
	public:
		cv::Rect region;
		//int left;	//領域左端
		//int top;	//領域上端
		//int right;	//領域右端
		//int bottom;	//領域下端
		float avg_vx;	//領域内移動ベクトルX成分
		float avg_vy;	//領域内移動ベクトルY成分
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
SHARED std::ostream& operator << (std::ostream& os, const cvImagePipeline::LabeledObject& labeledObject);
