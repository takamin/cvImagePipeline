#include <opencv2/opencv.hpp>
#include "ImageDataPtr.h"
namespace cvImagePipeline {
	__declspec(dllexport) 
	IplImage* createVideoAverageImage(CvCapture* capture) {
		CvSize frameSize;
		frameSize.width = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
		frameSize.height = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
		int frame_count = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
		IplImage* captured_frame = cvQueryFrame(capture);

		int depth = 0;
		int channels = 0;
		IplImage* sum = 0;
		ImageDataPtr sumPtr;
		ImageDataPtr imgPtr;
		if(captured_frame != 0) {
			depth = captured_frame->depth;
			channels = captured_frame->nChannels;
			sum = cvCreateImage(frameSize, IPL_DEPTH_32F, channels);
			sumPtr = sum;
		}

		//ピクセルごとの全フレームの総和
		while(captured_frame != 0) {
			imgPtr = captured_frame;
			for(int y = 0; y < frameSize.height; y++) {
				sumPtr.setLine(y);
				imgPtr.setLine(y);
				for(int x = 0; x < frameSize.width; x++) {
					for(int c = 0; c < channels; c++) {
						sumPtr = sumPtr + imgPtr;
						sumPtr++;
						imgPtr++;
					}
				}
			}
			captured_frame = cvQueryFrame(capture);
		}

		//平均化
		IplImage* avg = cvCreateImage(frameSize, depth, channels);

		sumPtr = sum;
		imgPtr = avg;
		for(int y = 0; y < frameSize.height; y++) {
			sumPtr.setLine(y);
			imgPtr.setLine(y);
			for(int x = 0; x < frameSize.width; x++) {
				for(int c = 0; c < channels; c++) {
					imgPtr = sumPtr / frame_count;
					sumPtr++;
					imgPtr++;
				}
			}
		}
		cvReleaseImage(&sum);
		cvSmooth(avg, avg);
		return avg;
	}
}
