#pragma once
#include <windows.h>
#include "ImgProcSet.h"
namespace cvImagePipeline {
	namespace Filter {

		// �Ɨ������X���b�h�ŉ摜�������s���v���Z�b�T
		class __declspec(dllexport) ImgProcThread : public ImgProcSet {
		public:
			DECLARE_CVFILTER;

			//�N���e�B�J���Z�N�V�����̃w���p�[
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
			
			//�X���b�h�̊J�n
			void startThread(int interval = 0);
			
			//�X���b�h�̏I��
			void stopThread();
			
			//�����҂�
			bool WaitEvent(DWORD timeout = INFINITE);

			//�X���b�h�Z�[�t�ȏo�͉摜�̎w��
			ImgProcThread& defThreadShareOutputMat(const std::string& name, ImageProcessor& src);
			ImgProcThread& defThreadShareOutputMat(const std::string& name, const cv::Mat& src);

			//�o�͉摜���X�V
			void updateThreadShareOutputMat();
			
			//�o�͉摜�̎Q�Ǝ擾
			const cv::Mat& getThreadShareOutputMat(const std::string& name) const;

			//�X���b�h�Z�[�t�ȏo�͉摜�̎w��
			void addThreadSafeOutput(const std::string& name, ImageProcessor& src) {
				defThreadShareOutputMat(name, src);
			}
			//�o�͉摜���X�V
			void ImgProcThread::updateSharedOutputMat() { updateThreadShareOutputMat(); }
			
			//�o�͉摜�̎Q�Ǝ擾
			const cv::Mat& refThreadShareOutput(const std::string& name) const {
				return getThreadShareOutputMat(name);
			}
		private:
			
			//�o�͉摜�̔r������p�N���e�B�J���Z�N�V����
			CRITICAL_SECTION cs;

			//�o�͉摜��ێ�����X���b�h�ԋ��L�v���Z�b�T
			ImgProcSet threadShareInnerMat;
			ImgProcSet threadShareOutputMat;
			
			//�T�u�X���b�h�̎��s�t���O
			bool runProcessorThread;
			
			//�T�u�X���b�h�̃n���h��
			HANDLE thead_handle;
			
			//�T�u�X���b�h�̎��s�C���^�[�o��
			int interval;

			//�o�͉摜�X�V�C�x���g
			HANDLE eventHandle;
		
		private:
			//���L�I�u�W�F�N�g�ւ̃A�N�Z�X�J�n
			void EnterCriticalSection();
			
			//���L�I�u�W�F�N�g�ւ̃A�N�Z�X�I��
			void LeaveCriticalSection();
			
			//�T�u�X���b�h�̃��[�g
			static DWORD WINAPI Thread(LPVOID data);

		};
	}
}
