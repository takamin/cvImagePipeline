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

			void ImgProcThread::startThread(int interval /* = 0 */) {
				eventHandle = CreateEvent(NULL, false, false, getName().c_str());
				runProcessorThread = true;
				this->interval = interval;
				thead_handle = ::CreateThread(0,0,
					ImgProcThread::Thread, this,
					0,NULL);
			}
			void ImgProcThread::stopThread() {
				runProcessorThread = false;
				WaitForSingleObject(thead_handle, INFINITE);
				CloseHandle(eventHandle);
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
			void ImgProcThread::updateThreadShareOutputMat() {
				CriticalSection lock(*this);
				threadShareOutputMat.execute();
			}
			ImgProcThread& ImgProcThread::defThreadShareOutputMat(const std::string& name, ImageProcessor& src) {
				threadShareInnerMat.add("ImagePoint", name, false);
				threadShareOutputMat.add("ImagePoint", name, false);
				threadShareInnerMat[name].setInputMat(src.getOutputMat());
				threadShareOutputMat[name].setInputMat(threadShareInnerMat[name].getOutputMat());
				return *this;
			}
			ImgProcThread& ImgProcThread::defThreadShareOutputMat(const std::string& name, const cv::Mat& src) {
				threadShareInnerMat.add("ImagePoint", name, false);
				threadShareOutputMat.add("ImagePoint", name, false);
				threadShareInnerMat[name].setInputMat(src);
				threadShareOutputMat[name].setInputMat(threadShareInnerMat[name].getOutputMat());
				return *this;
			}
			const cv::Mat& ImgProcThread::getThreadShareOutputMat(const std::string& name) const {
				return ((ImgProcSet&)threadShareOutputMat)[name].getOutputMat();
			}

			DWORD WINAPI ImgProcThread::Thread(LPVOID data) {
				ImgProcThread* processor = (ImgProcThread*)data;
#ifdef _DEBUG
				std::cerr << processor->getName() << " started." << std::endl;
#endif
				clock_t sleep_time = 0;
				long run_counter = 0;
				clock_t t0 = clock();
				while (processor->runProcessorThread) {
					if (run_counter > 0x7ffffff0) {
						run_counter = 0;
						t0 = clock();
					}
					if (processor->isEnable()){
						{
							CriticalSection lock(*processor);
							processor->execute();
							processor->threadShareInnerMat.execute();
						}
						::SetEvent(processor->eventHandle);
					}
					sleep_time += ((t0 + ++run_counter * processor->interval) - clock());
					if (sleep_time >= 0) {
						Sleep(sleep_time);
						sleep_time = 0;
					}
					else {
						Sleep(1);
						sleep_time -= 1;
					}
				}
#ifdef _DEBUG
				std::cerr << processor->getName() << " end." << std::endl;
#endif
				return 0;
			}
	}
}
