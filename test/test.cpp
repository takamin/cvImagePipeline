// test.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "ImageProcessor.h"
#include "AverageMat.h"

using namespace std;
using namespace cvUtils;
using namespace cvUtils::Filter;
struct test_counter {
	test_counter() : test_count(0), pass_count(0), fail_count(0) {}  
	int test_count;
	int pass_count;
	int fail_count;
};

void test(const string& name, bool result, test_counter& counter) {
	string result_str;
	counter.test_count++;
	if(result) {
		result_str = "PASS";
		counter.pass_count++;
	} else {
		result_str = "FAIL";
		counter.fail_count++;
	}
	cerr << "[" << result_str << "] test " << counter.test_count << ". " << name << endl;
}
int _tmain(int argc, _TCHAR* argv[])
{
	test_counter counter;

	ImgProcSet imgProcSet;
	test("imgProcSet.loadXml returns false when file not exists.",
		(false == imgProcSet.loadXml("not_exists.xml")),
		counter);

	test("imgProcSet.loadXml returns false when no xml file",
		(false == imgProcSet.loadXml("..\\ReadMe.txt")),
		counter);

	test("imgProcSet.loadXml returns true certain xml file",
		(true == imgProcSet.loadXml("..\\certain.xml")),
		counter);
	
	//while(true) {
	//	imgProcSet.execute();
	//	if(cvWaitKey(1) == '\x1b') {
	//		break;
	//	}
	//}


	/////////////////////////////////////////////////////////////////////////
	//
	// AverageMat
	//
	/////////////////////////////////////////////////////////////////////////

	// AverageMat 8UC1

	AverageMat averager8UC1;
	cv::Mat output;
	averager8UC1.averageCount = 2;
	cv::Mat avgInput8UC1_0(cv::Mat::zeros(3, 3, CV_8UC1));
	cv::Mat avgInput8UC1_1(cv::Mat::zeros(3, 3, CV_8UC1));
	cv::Mat avgInput8UC1_2(cv::Mat::zeros(3, 3, CV_8UC1));
	cv::Mat avgInput8UC1_3(cv::Mat::zeros(3, 3, CV_8UC1));

	avgInput8UC1_1 = avgInput8UC1_1 + 10;
	avgInput8UC1_2 = avgInput8UC1_2 + 20;
	avgInput8UC1_3 = avgInput8UC1_3 + 30;

	averager8UC1.setInputMat(avgInput8UC1_0);
	averager8UC1.execute();
	output = averager8UC1.getOutputMat();
	test("AverageMat output image rows equals to input",
		(output.rows == avgInput8UC1_0.rows), counter);
	test("AverageMat output image cols equals to input",
		(output.cols == avgInput8UC1_0.cols), counter);
	test("AverageMat output image type() equals to input",
		(output.type() == avgInput8UC1_0.type()), counter);

	test("AverageMat average[0] value(0,0)", (output.data[output.step*0+0] == 0), counter);
	test("AverageMat average[0] value(1,1)", (output.data[output.step*1+1] == 0), counter);
	test("AverageMat average[0] value(2,2)", (output.data[output.step*2+2] == 0), counter);
	
	averager8UC1.setInputMat(avgInput8UC1_1);
	averager8UC1.execute();
	output = averager8UC1.getOutputMat();
	test("AverageMat average[1] value(0,0)", (output.data[output.step*0+0] == 5), counter);
	test("AverageMat average[1] value(1,1)", (output.data[output.step*1+1] == 5), counter);
	test("AverageMat average[1] value(2,2)", (output.data[output.step*2+2] == 5), counter);

	averager8UC1.setInputMat(avgInput8UC1_2);
	averager8UC1.execute();
	output = averager8UC1.getOutputMat();
	test("AverageMat average[2] value(0,0)", (output.data[output.step*0+0] == 15), counter);
	test("AverageMat average[2] value(1,1)", (output.data[output.step*1+1] == 15), counter);
	test("AverageMat average[2] value(2,2)", (output.data[output.step*2+2] == 15), counter);

	averager8UC1.setInputMat(avgInput8UC1_3);
	averager8UC1.execute();
	output = averager8UC1.getOutputMat();
	test("AverageMat average[3] value(0,0)", (output.data[output.step*0+0] == 25), counter);
	test("AverageMat average[3] value(1,1)", (output.data[output.step*1+1] == 25), counter);
	test("AverageMat average[3] value(2,2)", (output.data[output.step*2+2] == 25), counter);


	// AverageMat 32FC3

	AverageMat averager32FC3;
	averager32FC3.averageCount = 2;
	cv::Mat avgInput32F3_0(cv::Mat::zeros(3, 3, CV_32FC3));
	cv::Mat avgInput32F3_1(cv::Mat::zeros(3, 3, CV_32FC3));
	cv::Mat avgInput32F3_2(cv::Mat::zeros(3, 3, CV_32FC3));
	cv::Mat avgInput32F3_3(cv::Mat::zeros(3, 3, CV_32FC3));

	avgInput32F3_1 = avgInput32F3_1 + 100.0;
	avgInput32F3_2 = avgInput32F3_2 + 200.0;
	avgInput32F3_3 = avgInput32F3_3 + 300.0;

	averager32FC3.setInputMat(avgInput32F3_0);
	averager32FC3.execute();
	output = averager32FC3.getOutputMat();
	int ch = output.channels();
	test("AverageMat output image rows equals to input",
		(output.rows == avgInput32F3_0.rows), counter);
	test("AverageMat output image cols equals to input",
		(output.cols == avgInput32F3_0.cols), counter);
	test("AverageMat output image type() equals to input",
		(output.type() == avgInput32F3_0.type()), counter);
	test("AverageMat output image channels() equals to input",
		(output.channels() == avgInput32F3_0.channels()), counter);

	test("AverageMat average[0] value(0,0)", (output.data[output.step*0+ch*0+0] == 0), counter);
	test("AverageMat average[0] value(1,1)", (output.data[output.step*1+ch*1+1] == 0), counter);
	test("AverageMat average[0] value(2,2)", (output.data[output.step*2+ch*2+2] == 0), counter);
	
	averager32FC3.setInputMat(avgInput32F3_1);
	averager32FC3.execute();
	output = averager32FC3.getOutputMat();
	test("AverageMat average[1] value(0,0)", (output.data[output.step*0+ch*0+0] == 50.0), counter);
	test("AverageMat average[1] value(1,1)", (output.data[output.step*1+ch*1+1] == 50.0), counter);
	test("AverageMat average[1] value(2,2)", (output.data[output.step*2+ch*2+2] == 50.0), counter);

	averager32FC3.setInputMat(avgInput32F3_2);
	averager32FC3.execute();
	output = averager32FC3.getOutputMat();
	test("AverageMat average[2] value(0,0)", (output.data[output.step*0+ch*0+0] == 150.0), counter);
	test("AverageMat average[2] value(1,1)", (output.data[output.step*1+ch*1+1] == 150.0), counter);
	test("AverageMat average[2] value(2,2)", (output.data[output.step*2+ch*2+2] == 150.0), counter);

	averager32FC3.setInputMat(avgInput32F3_3);
	averager32FC3.execute();
	output = averager32FC3.getOutputMat();
	test("AverageMat average[3] value(0,0)", (output.data[output.step*0+ch*0+0] == 250.0), counter);
	test("AverageMat average[3] value(1,1)", (output.data[output.step*1+ch*1+1] == 250.0), counter);
	test("AverageMat average[3] value(2,2)", (output.data[output.step*2+ch*2+2] == 250.0), counter);
	return 0;
}
