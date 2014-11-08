#include "stdafx.h"
#if defined(_MSC_VER)
#include <windows.h>
#else
#include <unistd.h>
#define Sleep(millisec) usleep(millisec * 1000)
#endif
#include <opencv2/opencv.hpp>
#include "cvImagePipeline.h"

using namespace cvImagePipeline;
using namespace cvImagePipeline::Filter;

#if defined(_MSC_VER)
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif
{
	cvInitSystem(argc, argv);
	ImgProcSet processors;
	std::string xml_filename("sample.xml");
	if (argc > 1) {
		xml_filename = argv[1];
	}
	if (!processors.loadXml(xml_filename)) {
		std::cerr << "ファイル読み込み失敗 ファイル名：" << xml_filename << std::endl;
		return -1;
	}
	Sleep(2000);
	while(true) {
		processors.execute();
		int ch = cvWaitKeyEx(1);
		if (ch == '\x1b') {
			fprintf(stderr, "exit.\n");
			break;
		}
	}
	cvDestroyAllWindows();
	return 0;
}
