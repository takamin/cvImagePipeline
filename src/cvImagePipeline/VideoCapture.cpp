#include "VideoCapture.h"
using namespace std;
namespace cvImagePipeline {
	namespace Filter {
		///////////////////////////////////////////////////////////////////////////
		// class VideoCapture

		IMPLEMENT_CVFILTER(VideoCapture);
		VideoCapture::VideoCapture()
		:	
			deviceIndex("deviceIndex", -1), filename("filename", ""),
			startFrame("startFrame", 0), stopFrame("stopFrame", -1),
			width("width", 0.0),
			height("height", 0.0),
			captureStart("captureStart", false),
			captureEmpty("captureEmpty", false),
			frameNumber(0)
		{
			defParam(deviceIndex);
			defParam(filename);
			defParam(startFrame);
			defParam(stopFrame);
			defParam(width);
			defParam(height);
			defParam(captureStart);
			defParam(captureEmpty);
			setInputMatDesc("", "‚±‚Ì“ü—Í‰æ‘œ‚ÍŽg—p‚³‚ê‚Ü‚¹‚ñ");
		}
		VideoCapture::~VideoCapture() { }
		bool VideoCapture::open(int deviceIndex)
		{
			bool open_state = videoCapture.open(deviceIndex);

			if(width != 0.0) {
				videoCapture.set(CV_CAP_PROP_FRAME_WIDTH, width);
			}
			if(height != 0.0) {
				videoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, height);
			}
			captureStart = false;
			return open_state;
		}
		bool VideoCapture::open(const std::string& filename)
		{ 
			bool open_state = videoCapture.open(filename);
			if(width != 0.0) {
				videoCapture.set(CV_CAP_PROP_FRAME_WIDTH, width);
			}
			if(height != 0.0) {
				videoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, height);
			}
			captureStart = false;
			return open_state;
		}
		bool VideoCapture::capture() {
			if(!videoCapture.isOpened()) {
				if(deviceIndex >= 0) {
					open(deviceIndex);
				} else if((string)filename != "") {
					open((string)filename);
				} else {
					return false;
				}
			}
			cv::Mat& mat = refOutputMat(); 
			if (captureStart == false) {
				do {
					videoCapture >> mat;
					frameNumber++;
					_sleep(10);
				} while (mat.empty());
				captureStart = true;
			}
			else if (!captureEmpty) {
				while (frameNumber < startFrame) {
					videoCapture >> mat;
					frameNumber++;
					if (mat.empty() || frameNumber >= stopFrame) {
						captureEmpty = true;
						return true;
					}
				}
				if (stopFrame < 0 || frameNumber < stopFrame) {
					videoCapture >> mat;
					frameNumber++;
					if (mat.empty() || (stopFrame >= 0 && frameNumber >= stopFrame)) {
						captureEmpty = true;
						return true;
					}
				}
			}
			return true;
		}
		void VideoCapture::execute() {
			capture();
		}
	}
}
