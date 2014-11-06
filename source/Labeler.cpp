#include "Labeler.h"
namespace cvImagePipeline {
	namespace Filter {
		using namespace std;
		using namespace cv;
		IMPLEMENT_CVFILTER(Labeler);

		Labeler::Labeler()
			: labeling_region_min_size("labeling_region_min_size", 1000)
		{
			defParam(labeling_region_min_size);
		}
		Labeler::~Labeler()
		{
		}
		void Labeler::execute() {
			const Mat& flow = getInputMat();
			labeling.Exec(
				buffer->label_src, buffer->label_result,
				flow.cols, flow.rows,
				true, labeling_region_min_size);

			int label_result_count = labeling.GetNumOfResultRegions();
			labeledObjects.clear();
			for(int i = 0; i < label_result_count; i++) {
				LabelingBS::RegionInfo* regionInfo = labeling.GetResultRegionInfo(i);
				LabeledObject labeledObject;
				regionInfo->GetMin(labeledObject.region.x, labeledObject.region.y);
				regionInfo->GetSize(labeledObject.region.width, labeledObject.region.height);
				labeledObjects.push_back(labeledObject);
			}
		}

		static const int INITIAL_BUFSIZE = 640 * 480;
		Labeler::Buffer::Buffer()
			: bufsize(INITIAL_BUFSIZE),
			label_src(new unsigned char[INITIAL_BUFSIZE]),
			label_result(new short[INITIAL_BUFSIZE])
		{
		}

	}
}
