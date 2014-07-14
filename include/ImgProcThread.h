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
			
			//�摜�����̎��s
			void execute();
			
			//�X���b�h�̊J�n
			void startThread(int interval = 0);
			
			//�X���b�h�̏I��
			void stopThread();
			
			//�X���b�h�Z�[�t�ȏo�͉摜�̎w��
			void addThreadSafeOutput(std::string name, ImageProcessor& src);

			//�o�͉摜�̎擾
			cv::Mat getThreadShareOutput(std::string name) const;

			//�����҂�
			bool ImgProcThread::WaitEvent(DWORD timeout = INFINITE);

			//�����҂��C�x���g�N���A
			void ImgProcThread::ResetEvent();

			//���L�I�u�W�F�N�g�ւ̃A�N�Z�X�J�n
			void EnterCriticalSection();
			
			//���L�I�u�W�F�N�g�ւ̃A�N�Z�X�I��
			void LeaveCriticalSection();
			
		private:
			
			//�o�͉摜�̔r������p�N���e�B�J���Z�N�V����
			CRITICAL_SECTION cs;

			//�o�͉摜��ێ�����X���b�h�ԋ��L�v���Z�b�T
			ImgProcSet threadShare;
			
			//�T�u�X���b�h�̎��s�t���O
			bool runProcessorThread;
			
			//�T�u�X���b�h�̃n���h��
			HANDLE thead_handle;
			
			//�T�u�X���b�h�̎��s�C���^�[�o��
			int interval;

			//�o�͉摜�X�V�C�x���g
			HANDLE eventHandle;
		
		private:
			//�T�u�X���b�h�̃��[�g
			static DWORD WINAPI Thread(LPVOID *data);

		};
	}
}
