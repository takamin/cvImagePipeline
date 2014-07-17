#include "ImgProcThread.h"
#include <time.h>

namespace cvImagePipeline {
	namespace Filter {
			IMPLEMENT_CVFILTER(ImgProcThread);

			ImgProcThread::ImgProcThread()
			: runProcessorThread(false), thead_handle(INVALID_HANDLE_VALUE), interval(0),
				eventHandle(INVALID_HANDLE_VALUE)
			{
				::InitializeCriticalSection(&cs);
			}
			ImgProcThread::~ImgProcThread() {
				::DeleteCriticalSection(&cs);
			}

			void ImgProcThread::addThreadSafeOutput(const std::string& name, ImageProcessor& src) {
				threadShareInnerMat.add("ImagePoint", name, false);
				threadShareOutputMat.add("ImagePoint", name, false);
				threadShareInnerMat[name].setInputMat(src.getOutputMat());
				threadShareOutputMat[name].setInputMat(threadShareInnerMat[name].getOutputMat());
			}
			void ImgProcThread::startThread(int interval /* = 0 */) {
				eventHandle = CreateEvent(NULL, false, false, getName().c_str());
				runProcessorThread = true;
				this->interval = interval;
				thead_handle = ::CreateThread(0,0,
					(LPTHREAD_START_ROUTINE)ImgProcThread::Thread,(LPVOID)this,
					0,NULL);
			}
			void ImgProcThread::stopThread() {
				runProcessorThread = false;
				WaitForSingleObject(thead_handle, INFINITE);
				CloseHandle(eventHandle);
			}
			void ImgProcThread::execute() {

				ImgProcSet::execute();

				EnterCriticalSection();
				threadShareInnerMat.execute();
				LeaveCriticalSection();

				SetEvent(eventHandle);
			}
			void ImgProcThread::EnterCriticalSection() {
				::EnterCriticalSection(&cs);
			}
			void ImgProcThread::LeaveCriticalSection() {
				::LeaveCriticalSection(&cs);
			}

			bool ImgProcThread::WaitEvent(DWORD timeout) {
				DWORD waitResult = WaitForSingleObject(eventHandle, timeout);
				return (waitResult == WAIT_OBJECT_0);
			}
			void ImgProcThread::updateSharedOutputMat() {
				EnterCriticalSection();
				threadShareOutputMat.execute();
				LeaveCriticalSection();
			}
			const cv::Mat& ImgProcThread::refThreadShareOutput(const std::string& name) const {
				return ((ImgProcSet&)threadShareOutputMat)[name].getOutputMat();
			}

			DWORD WINAPI ImgProcThread::Thread(LPVOID *data) {
				ImgProcThread* processor = (ImgProcThread*)data;
#ifdef _DEBUG
				std::cerr << processor->getName() << " started." << std::endl;
#endif
				clock_t t0 = clock();
				while(processor->runProcessorThread) {
					processor->execute();
					if(processor->interval > 0) {
						clock_t t1;
						while((t1 = clock()) - t0 < processor->interval) {
							Sleep(processor->interval / 10);
						}
						t0 = t1;
					} else {
						Sleep(0);
					}
				}
#ifdef _DEBUG
				std::cerr << processor->getName() << " end." << std::endl;
#endif
				return 0;
			}
	}
}
