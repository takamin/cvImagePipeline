#include "LabeledObject.h"
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <cv.h>
#include <ctime>

using namespace std;
using namespace cv;

namespace cvImagePipeline {

	LabeledObject::LabeledObject() : region(0,0,0,0), avg_vx(0), avg_vy(0), clock(::clock()) { }
	bool LabeledObject::operator==(const LabeledObject& that) const {
		return (
			region == that.region
			&&  avg_vx == that.avg_vx
			&&  avg_vy == that.avg_vy
			);
	}
	void LabeledObject::putTo(std::ostream& os) const {
		os << clock << ", ";
		os << region;
		os << avg_vx << ", " << avg_vy << ", ";
		os << sqrt(avg_vx*avg_vx + avg_vy*avg_vy) << ", ";
	}
	void LabeledObject::chooseByArea(std::list<LabeledObject>& obj_list, double max_area_size) {
		list<LabeledObject>::iterator obj = obj_list.begin();
		while(obj != obj_list.end()) {
			if(obj->size().area() > max_area_size) {
				obj = obj_list.erase(obj);
			} else {
				obj++;
			}
		}
	}
}
std::ostream& operator << (std::ostream& os, const cvImagePipeline::LabeledObject& obj) {
	obj.putTo(os);
	return os;
}
