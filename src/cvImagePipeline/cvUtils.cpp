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
			fprintf(stderr, "もう一度ESCを押せば終了。それ以外で再開。");
			while((ch = cvWaitKey(100)) == -1) {/**/}
		}
		return ch;
	}
}