#pragma once
#include <windows.h>
#include "ImgProcSet.h"
namespace cvImagePipeline {
	namespace Filter {
		class __declspec(dllexport) ImgProcThread : public ImgProcSet {
		public:
			DECLARE_CVFILTER;
		public:
			ImgProcThread();
			~ImgProcThread();
			
			//画像処理の実行
			void execute();
			
			//スレッドの開始
			void startThread(int interval = 0);
			
			//スレッドの終了
			void stopThread();
			
			//スレッドセーフな出力画像の指定
			void addThreadSafeOutput(std::string name, ImageProcessor& src);

			//出力画像の取得
			cv::Mat getThreadShareOutput(std::string name) const;

			//処理待ち
			bool ImgProcThread::WaitEvent(DWORD timeout = INFINITE);

			//処理待ちイベントクリア
			void ImgProcThread::ResetEvent();

			//共有オブジェクトへのアクセス開始
			void EnterCriticalSection();
			
			//共有オブジェクトへのアクセス終了
			void LeaveCriticalSection();
			
		private:
			
			//出力画像の排他制御用クリティカルセクション
			CRITICAL_SECTION cs;

			//出力画像を保持するスレッド間共有プロセッサ
			ImgProcSet threadShare;
			
			//サブスレッドの実行フラグ
			bool runProcessorThread;
			
			//サブスレッドのハンドル
			HANDLE thead_handle;
			
			//サブスレッドの実行インターバル
			int interval;

			//出力画像更新イベント
			HANDLE eventHandle;
		
		private:
			//サブスレッドのルート
			static DWORD WINAPI Thread(LPVOID *data);

		};
	}
}
