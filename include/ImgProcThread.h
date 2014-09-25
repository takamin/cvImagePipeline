#pragma once
#include <windows.h>
#include "ImgProcSet.h"
namespace cvImagePipeline {
	namespace Filter {

		// 独立したスレッドで画像処理を行うプロセッサ
		class __declspec(dllexport) ImgProcThread : public ImgProcSet {
		public:
			DECLARE_CVFILTER;

			//クリティカルセクションのヘルパー
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
			
			//スレッドの開始
			void startThread(int interval = 0);
			
			//スレッドの終了
			void stopThread();
			
			//処理待ち
			bool WaitEvent(DWORD timeout = INFINITE);

			//スレッドセーフな出力画像の指定
			ImgProcThread& defThreadShareOutputMat(const std::string& name, ImageProcessor& src);
			ImgProcThread& defThreadShareOutputMat(const std::string& name, const cv::Mat& src);

			//出力画像を更新
			void updateThreadShareOutputMat();
			
			//出力画像の参照取得
			const cv::Mat& getThreadShareOutputMat(const std::string& name) const;

			//スレッドセーフな出力画像の指定
			void addThreadSafeOutput(const std::string& name, ImageProcessor& src) {
				defThreadShareOutputMat(name, src);
			}
			//出力画像を更新
			void ImgProcThread::updateSharedOutputMat() { updateThreadShareOutputMat(); }
			
			//出力画像の参照取得
			const cv::Mat& refThreadShareOutput(const std::string& name) const {
				return getThreadShareOutputMat(name);
			}
		private:
			
			//出力画像の排他制御用クリティカルセクション
			CRITICAL_SECTION cs;

			//出力画像を保持するスレッド間共有プロセッサ
			ImgProcSet threadShareInnerMat;
			ImgProcSet threadShareOutputMat;
			
			//サブスレッドの実行フラグ
			bool runProcessorThread;
			
			//サブスレッドのハンドル
			HANDLE thead_handle;
			
			//サブスレッドの実行インターバル
			int interval;

			//出力画像更新イベント
			HANDLE eventHandle;
		
		private:
			//共有オブジェクトへのアクセス開始
			void EnterCriticalSection();
			
			//共有オブジェクトへのアクセス終了
			void LeaveCriticalSection();
			
			//サブスレッドのルート
			static DWORD WINAPI Thread(LPVOID data);

		};
	}
}
