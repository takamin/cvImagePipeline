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

			void ImgProcThread::EnterCriticalSection() {
				::EnterCriticalSection(&cs);
			}
			void ImgProcThread::LeaveCriticalSection() {
				::LeaveCriticalSection(&cs);
			}
			void ImgProcThread::execute() {

				ImgProcSet::execute();

				EnterCriticalSection();
				threadShare.execute();
				LeaveCriticalSection();

				SetEvent(eventHandle);
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

			bool ImgProcThread::WaitEvent(DWORD timeout) {
				DWORD waitResult = WaitForSingleObject(eventHandle, timeout);
				return (waitResult == WAIT_OBJECT_0);
			}
			void ImgProcThread::ResetEvent() {
				::ResetEvent(eventHandle);
			}

			void ImgProcThread::addThreadSafeOutput(std::string name, ImageProcessor& src) {
				threadShare.add("ImagePoint", name, false);
				threadShare[name].setInputMat(src.getOutputMat());
			}
			cv::Mat ImgProcThread::getThreadShareOutput(std::string name) const {
				return ((ImgProcSet&)threadShare)[name].getOutputMat().clone();
			}

			DWORD WINAPI ImgProcThread::Thread(LPVOID *data) {
				ImgProcThread* processor = (ImgProcThread*)data;
				std::cerr << processor->getName() << " started." << std::endl;
				clock_t t0 = clock();
				while(processor->runProcessorThread) {
					processor->execute();
					//Sleep(processor->interval);
					if(processor->interval > 0) {
						clock_t t1;
						//int wait_count = 0;
						while((t1 = clock()) - t0 < processor->interval) {
							Sleep(processor->interval / 10);
							//wait_count++;
						}
						//if(wait_count == 0) {
						//	std::cerr << "Thread " << processor->getName() << " overrun " << ((t1 - t0) - processor->interval) << " [ms]" << std::endl;
						//}
						t0 = t1;
					} else {
						Sleep(0);
					}
				}
				std::cerr << processor->getName() << " end." << std::endl;
				return 0;
			}
	}
}
