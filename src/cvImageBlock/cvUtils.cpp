#include <windows.h>
#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/nonfree.hpp>

BOOL WINAPI DllMain( HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved)
{
	cv::initModule_nonfree(); 
	return TRUE;
}

namespace cvUtils {
	__declspec(dllexport)
	int cvWaitKeyEx(int delay /* =1 */) {
		int ch = cvWaitKey(delay);
		if (ch == '\x1b') {
			fprintf(stderr, "������xESC�������ΏI���B����ȊO�ōĊJ�B");
			while((ch = cvWaitKey(100)) == -1) {/**/}
		}
		return ch;
	}
}