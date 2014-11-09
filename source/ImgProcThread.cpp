#include "ImgProcThread.h"
#include <time.h>
#if !defined(_MSC_VER)
#include <unistd.h>
#define Sleep(ms) usleep((ms)*1000)
#endif

namespace cvImagePipeline {
	namespace Filter {
			IMPLEMENT_CVFILTER(ImgProcThread);

			ImgProcThread::ImgProcThread()
			: runProcessorThread(false), thead_handle(INVALID_HANDLE_VALUE), interval(0),
				eventHandle(INVALID_HANDLE_VALUE)
			{
#if defined(_MSC_VER)
				::InitializeCriticalSection(&cs);
#else
#endif
			}
			ImgProcThread::~ImgProcThread() {
#if defined(_MSC_VER)
				::DeleteCriticalSection(&cs);
#else
#endif
			}

			void ImgProcThread::startThread(int interval /* = 0 */) {
#if defined(_MSC_VER)
				eventHandle = CreateEvent(NULL, false, false, getName().c_str());
#else
#endif
				runProcessorThread = true;
				this->interval = interval;
#if defined(_MSC_VER)
				thead_handle = ::CreateThread(0,0,
					ImgProcThread::Thread, this,
					0,NULL);
#else
#endif
			}
			void ImgProcThread::stopThread() {
				runProcessorThread = false;
#if defined(_MSC_VER)
				WaitForSingleObject(thead_handle, INFINITE);
				CloseHandle(eventHandle);
#else
#endif
			}
			void ImgProcThread::EnterCriticalSection() {
#if defined(_MSC_VER)
				::EnterCriticalSection(&cs);
#else
#endif
			}
			void ImgProcThread::LeaveCriticalSection() {
#if defined(_MSC_VER)
				::LeaveCriticalSection(&cs);
#else
#endif
			}
			bool ImgProcThread::WaitEvent(DWORD timeout) {
#if defined(_MSC_VER)
				DWORD waitResult = WaitForSingleObject(eventHandle, timeout);
				return (waitResult == WAIT_OBJECT_0);
#else
                return true;
#endif
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
#if defined(_MSC_VER)
						::SetEvent(processor->eventHandle);
#else
#endif
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
