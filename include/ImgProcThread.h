#pragma once
#include <windows.h>
#include "ImgProcSet.h"
namespace cvImagePipeline {
	namespace Filter {

		// Threading processor
		class __declspec(dllexport) ImgProcThread : public ImgProcSet {
		public:
			DECLARE_CVFILTER;

			// Helper for critical section
			class CriticalSection {
				ImgProcThread& imgProcThread;
			public:
				CriticalSection(ImgProcThread& imgProcThread) : imgProcThread(imgProcThread)
				{ imgProcThread.EnterCriticalSection(); }
				~CriticalSection() { imgProcThread.LeaveCriticalSection(); }
			};
		public:
			ImgProcThread();
			~ImgProcThread();
			
			void startThread(int interval = 0);
			
			void stopThread();
			
			bool WaitEvent(DWORD timeout = INFINITE);

			//declare thread-safe output image
			ImgProcThread& defThreadShareOutputMat(const std::string& name, ImageProcessor& src);
			ImgProcThread& defThreadShareOutputMat(const std::string& name, const cv::Mat& src);

			//update output images
			void updateThreadShareOutputMat();
			
			//get ref to output image
			const cv::Mat& getThreadShareOutputMat(const std::string& name) const;

			//declare thread-safe output image
			void addThreadSafeOutput(const std::string& name, ImageProcessor& src) {
				defThreadShareOutputMat(name, src);
			}
			//update output images
			void ImgProcThread::updateSharedOutputMat() { updateThreadShareOutputMat(); }
			
			//get ref to output image
			const cv::Mat& refThreadShareOutput(const std::string& name) const {
				return getThreadShareOutputMat(name);
			}
		private:
			
			CRITICAL_SECTION cs;

			//processor that composit thread-share output images
			ImgProcSet threadShareInnerMat;
			ImgProcSet threadShareOutputMat;
			
			// thread property
			bool runProcessorThread;
			HANDLE thead_handle;
			int interval;

			//event that output images are updated
			HANDLE eventHandle;
		
		private:
			void EnterCriticalSection();
			void LeaveCriticalSection();
			
			//thread main
			static DWORD WINAPI Thread(LPVOID data);

		};
	}
}
