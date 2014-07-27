#include "stdafx.h"
#include <windows.h>
#include <opencv2/opencv.hpp>
#include "cvImagePipeline.h"

using namespace cvImagePipeline;
using namespace cvImagePipeline::Filter;

int _tmain(int argc, _TCHAR* argv[])
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
