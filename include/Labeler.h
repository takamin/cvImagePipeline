#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
#include "ImageProcessor.h"
#include "LabeledObject.h"
#include "Labeling.h"
#pragma warning(disable:4251)

namespace cvUtils {
	namespace Filter {
		class __declspec(dllexport) Labeler : public ImageProcessor
		{
		public:
			DECLARE_CVFILTER;
			class  __declspec(dllexport) Buffer {
				friend class Labeler;
			protected:
				size_t bufsize;
				unsigned char* label_src;
				short* label_result;
			public:
				Buffer();
				virtual ~Buffer() {
					delete [] label_src;
					delete [] label_result;
				}
				void reallocate(const cv::Mat& inputMat) {
					if((size_t)inputMat.cols * inputMat.rows > bufsize) {
						delete [] label_src;
						delete [] label_result;
						bufsize = inputMat.cols * inputMat.rows;
						label_src = new unsigned char[bufsize];
						label_result = new short[bufsize];
					}
				}
				void setLabelingSourceBuffer(const cv::Mat& image) {
					reallocate(image);
					int y,x,ch,src_ch = image.channels();
					unsigned value;
					for(y = 0; y < image.rows; y++) {
						unsigned char* src = image.data + y * image.step;
						unsigned char* dst = label_src + y * image.cols;
						for(x = 0; x < image.cols; x++, dst++) {
							for(ch = 0, value = 0; ch < src_ch; ch++, src++) {
								value += (unsigned)*src;
							}
							if(value != 0) {
								*dst = 1;
							} else {
								*dst = 0;
							}
						}
					}
				}
			};
			Buffer* buffer;
		public:
			TParam<int> labeling_region_min_size;
		public:
			Labeler();
			virtual ~Labeler();
			void setBuffer(Buffer * buffer) {this->buffer = buffer;}
			virtual void execute();
			const std::list<LabeledObject>& getLabeledObjects() const
			{ return labeledObjects; }
		private:
			void MakeLabelingSourceBuffer();
		private:
			std::list<LabeledObject> labeledObjects;
			LabelingBS labeling;
		};
	}
}
#pragma warning(default:4251)
