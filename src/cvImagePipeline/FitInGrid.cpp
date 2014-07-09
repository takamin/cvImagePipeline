#include "FitInGrid.h"
using namespace std;
namespace cvImagePipeline {
	namespace Filter {
		///////////////////////////////////////////////////////////////////////////
		// class FitInGrid

		IMPLEMENT_CVFILTER(FitInGrid);
		FitInGrid::FitInGrid()
			: 
				width("width", 640), height("height", 480),
				cols("cols", 2), rows("rows", 2)
		{
			defParam(width);
			defParam(height);
			defParam(cols);
			defParam(rows);
			undefInputMat("");
			refresh();
		}
		FitInGrid::~FitInGrid() {
#ifdef _DEBUG
			std::cerr << "FitInGrid::~FitInGrid(): ";
#endif
			int curr_count = dynInputMat.size();
			for(int i = 0; i < curr_count; i++) {
#ifdef _DEBUG
				std::cerr << "delete dynInputMat[" << i << "];";
#endif
				delete dynInputMat.at(i);
				dynInputMat.at(i) = 0;
			}
		}
		void FitInGrid::execute() {
			for(int row = 0; row < rows; row++) {
				for(int col = 0; col < cols; col++) {
					int index = row * rows + col;
					stringstream ss;
					ss << index;
					std::string inputMatName = ss.str();
					const cv::Mat& inputMat = getInputMat(inputMatName);
					if(!inputMat.empty()) {
						cv::Mat grid(refOutputMat(), cv::Rect(col * gridW, row * gridH, gridW, gridH));
						
						cv::Mat temp;
						double alpha = 1.0;
						double beta = 0.0;
						switch(inputMat.depth()) {
						case CV_8S: alpha = 1.0; beta = 128.0; break;
						case CV_16U: alpha = 1.0 / 0x10000; beta = 0.0; break;
						case CV_16S: alpha = 1.0 / 0x8000; beta = 128.0; break;
						case CV_32F: alpha = 255.0; beta = 0.0; break;
						}
						inputMat.convertTo(temp, CV_8UC3, alpha, beta);
						if(temp.channels() != 3) {
							cv::Mat temp2;
							cv::cvtColor(temp, temp2, CV_GRAY2BGR, 3);
							cv::resize(temp2, grid, cv::Size(gridW, gridH), 0.0, 0.0, CV_INTER_CUBIC);
						} else {
							cv::resize(temp, grid, cv::Size(gridW, gridH), 0.0, 0.0, CV_INTER_CUBIC);
						}
					}
				}
			}
		}
		void FitInGrid::onPropertyChange(Property& property) {
			refresh();
		}
		void FitInGrid::refresh() {
#ifdef _DEBUG
			std::cerr << "FitInGrid::recreateInputMats(): ";
#endif
			gridW = width / cols;
			gridH = height / rows;
			refOutputMat() = cv::Mat::zeros(height, width, CV_8UC3);

			int curr_count = dynInputMat.size();
			int next_count = (int)cols * (int)rows;

#ifdef _DEBUG
			std::cerr << "curr_count==" << curr_count << ";";
			std::cerr << "next_count==" << next_count << ";";
#endif
			if(next_count < curr_count) {
				for(int i = next_count; i < curr_count; i++) {
					stringstream ss;
					ss << i;
					std::string name = ss.str();
					undefInputMat(name);
					delete dynInputMat[i];
					dynInputMat[i] = 0;
#ifdef _DEBUG
					std::cerr << "undefInputMat(" << name << ");";
					std::cerr << "delete #" << i << ";";
#endif
				}
				while(dynInputMat.size() > next_count) {
					dynInputMat.pop_back();
				}
#ifdef _DEBUG
					std::cerr << "new dynInputMat.size()==" << "dynInputMat.size()" << ";";
#endif
			} else if(next_count > curr_count) {
				for(int i = curr_count; i < next_count; i++) {
#ifdef _DEBUG
					std::cerr << "dynInputMat[" << i << "] = new cv::Mat() " << ";";
#endif
					cv::Mat* inputMat = new cv::Mat(cv::Mat::zeros(gridH, gridW, CV_8UC3));
					dynInputMat.push_back(inputMat);

					std::stringstream ss;
					ss << i;
					std::string name = ss.str();
					defInputMat(name);
					setInputMat(name, *inputMat);
				}
			}
		}
		void FitInGrid::onInputMatConnected(const std::string& inputMatName) {
#ifdef _DEBUG
			std::cerr << "FitInGrid::onInputMatConnected(): ";
#endif
			stringstream ss(inputMatName);
			int index;
			ss >> index;
			const cv::Mat& inputMat = getInputMat(inputMatName);
			if(dynInputMat[index] != 0 && dynInputMat[index] != &inputMat) {
#ifdef _DEBUG
				std::cerr << "delete dynInputMat[" << index << "];";
#endif
				delete dynInputMat[index];
				dynInputMat[index] = 0;
			}
		}
	}
}
