#include <windows.h>
#include "OpticalFlowFarneback.h"

namespace cvImagePipeline {
	namespace Filter {

		using namespace std;
		using namespace cv;

		IMPLEMENT_CVFILTER(OpticalFlowFarneback);
		OpticalFlowFarneback::OpticalFlowFarneback()
			:
				pyr_scale("pyr_scale", 0.5),
				levels("levels", 1),
				winsize("winsize", 15),
				iterations("iterations", 1),
				poly_n("poly_n", 5),
				poly_sigma("poly_sigma", 1.1),
				flags("flags", 0
					//|OPTFLOW_USE_INITIAL_FLOW
					//|OPTFLOW_LK_GET_MIN_EIGENVALS
					|OPTFLOW_FARNEBACK_GAUSSIAN
				)
		{
			defParam(pyr_scale);
			defParam(levels);
			defParam(winsize);
			defParam(iterations);
			defParam(poly_n);
			defParam(poly_sigma);
			defParam(flags);
		}
		void OpticalFlowFarneback::reset() 
		{
			prevMat = Mat();
		}
		void OpticalFlowFarneback::execute() {
			const Mat& inputMat = getInputMat();
			if(!prevMat.empty()) {
				//assert(inputMat->channels() != 1);
				calc(prevMat, inputMat, refOutputMat());
			}
			prevMat = inputMat.clone();
		}

		void OpticalFlowFarneback::calc(const Mat& prev, const Mat& next, Mat& flow)
		{
			cv::calcOpticalFlowFarneback(
				prev, next, flow, 
				pyr_scale, levels, winsize,
				iterations, poly_n, poly_sigma,
				flags);
		}




		void OpticalFlowVisualizer::execute() {
			const Mat& flow = getInputMat();
			Mat visual_flow(flow.rows, flow.cols, CV_32FC3);
			int dst_ch = visual_flow.channels();
			int src_ch = flow.channels();
			for(int y = 0; y < flow.rows; y++) {
				float* psrc = (float*)(flow.data + flow.step * y);
				float* pdst = (float*)(visual_flow.data + visual_flow.step * y);
				for(int x = 0; x < flow.cols; x++) {
					float dx = psrc[0];
					float dy = psrc[1];
					
					//   ªÂ
					//Ô©¨—Î
					//   «
					//   ‰©F
					float r = (dx < 0.0) ? abs(dx) : 0;
					float g = (dx > 0.0) ? dx : 0;
					float b = (dy < 0.0) ? abs(dy) : 0;
					r += (dy > 0.0) ? dy : 0;
					g += (dy > 0.0) ? dy : 0;

					if(dx * dx + dy * dy < 0.0) {
						r = g = b = 0.0;
					}

					pdst[0] = b;
					pdst[1] = g;
					pdst[2] = r;

					pdst += dst_ch;
					psrc += src_ch;
				}
			}
			setOutputMat(visual_flow);
		}
	}
}
