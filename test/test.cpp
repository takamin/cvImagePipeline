// test.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "ImageProcessor.h"
#include "AverageMat.h"
#include "SubMat.h"

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

template<class T> bool is_equal(const T& a, const T& b)
{
	return (a == b);
}
bool is_equal(const float& a, const float& b) {
	return (abs(a - b) < FLT_EPSILON);
}
bool is_equal(const double& a, const double& b) {
	return (abs(a - b) < DBL_EPSILON);
}
template<class PixelT>bool testPixel(const cv::Mat& image, int x, int y, int ch, PixelT test_value) {
	PixelT* p = (PixelT*)(image.data + image.step * y);
	p += x * image.channels();
	if(is_equal(p[ch], test_value)) {
		return true;
	}
	cerr << "test fail: expected " << test_value << " at (" << x << ", " << y << ", " << ch << "), but " << p[ch] << std::endl;
	return false;
}
bool testPixel(const cv::Mat& image, int x, int y, int ch, unsigned char test_value) {
	unsigned char* p = (unsigned char*)(image.data + image.step * y);
	p += x * image.channels();
	if(is_equal(p[ch], test_value)) {
		return true;
	}
	cerr << "test fail: expected " << (int)test_value << " at (" << x << ", " << y << ", " << ch << "), but " << (int)p[ch] << std::endl;
	return false;
}
bool testPixel(const cv::Mat& image, int x, int y, int ch, char test_value) {
	char* p = (char*)(image.data + image.step * y);
	p += x * image.channels();
	if(is_equal(p[ch], test_value)) {
		return true;
	}
	cerr << "test fail: expected " << (int)test_value << " at (" << x << ", " << y << ", " << ch << "), but " << (int)p[ch] << std::endl;
	return false;
}
int _tmain(int argc, _TCHAR* argv[])
{
	test_counter counter;

	ImgProcSet imgProcSet;
	test("imgProcSet.loadXml returns false when file not exists.",
		(false == imgProcSet.loadXml("not_exists.xml")),
		counter);

	test("imgProcSet.loadXml returns false when no xml file",
		(false == imgProcSet.loadXml("..\\..\\test\\ReadMe.txt")),
		counter);

	test("imgProcSet.loadXml returns true certain xml file",
		(true == imgProcSet.loadXml("..\\..\\test\\certain.xml")),
		counter);
	
	while(true) {
		imgProcSet.execute();
		if(cvWaitKey(1) == '\x1b') {
			break;
		}
	}


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

	avgInput32F3_1 = cv::Scalar(100.0, 100.0, 100.0);
	avgInput32F3_2 = cv::Scalar(200.0, 200.0, 200.0);
	avgInput32F3_3 = cv::Scalar(300.0, 300.0, 300.0);

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

	test("AverageMat average[0] value(0,0)", testPixel<float>(output, 0, 0, 0, 0.0), counter);
	test("AverageMat average[0] value(1,1)", testPixel<float>(output, 1, 1, 1, 0.0), counter);
	test("AverageMat average[0] value(2,2)", testPixel<float>(output, 2, 2, 2, 0.0), counter);
	
	averager32FC3.setInputMat(avgInput32F3_1);
	averager32FC3.execute();
	output = averager32FC3.getOutputMat();
	test("AverageMat average[1] value(0,0)", testPixel<float>(output, 0, 0, 0, 50.0), counter);
	test("AverageMat average[1] value(1,1)", testPixel<float>(output, 1, 1, 1, 50.0), counter);
	test("AverageMat average[1] value(2,2)", testPixel<float>(output, 2, 2, 2, 50.0), counter);

	averager32FC3.setInputMat(avgInput32F3_2);
	averager32FC3.execute();
	output = averager32FC3.getOutputMat();
	test("AverageMat average[2] value(0,0)", testPixel<float>(output, 0, 0, 0, 150.0), counter);
	test("AverageMat average[2] value(1,1)", testPixel<float>(output, 1, 1, 1, 150.0), counter);
	test("AverageMat average[2] value(2,2)", testPixel<float>(output, 2, 2, 2, 150.0), counter);

	averager32FC3.setInputMat(avgInput32F3_3);
	averager32FC3.execute();
	output = averager32FC3.getOutputMat();
	test("AverageMat average[3] value(0,0)", testPixel<float>(output, 0, 0, 0, 250.0), counter);
	test("AverageMat average[3] value(1,1)", testPixel<float>(output, 1, 1, 1, 250.0), counter);
	test("AverageMat average[3] value(2,2)", testPixel<float>(output, 2, 2, 2, 250.0), counter);

	// SubMat 32FC3

	SubMat sub32FC3;
	cv::Mat subInput32F3_0;
	cv::Mat subInput32F3_1;

	subInput32F3_0 = cv::Mat::zeros(3, 3, CV_32FC3);
	subInput32F3_1 = cv::Mat::zeros(3, 2, CV_32FC3);
	sub32FC3.setInputMat(subInput32F3_0);
	sub32FC3.setInputMat("subImage", subInput32F3_1);
	sub32FC3.execute();
	output = sub32FC3.getOutputMat();
	test("SubMat cannot calc between differ image. output image is empty()",
		(output.empty()), counter);

	subInput32F3_0 = cv::Mat::zeros(3, 3, CV_32FC3);
	subInput32F3_1 = cv::Mat::zeros(3, 3, CV_32FC3);
	sub32FC3.setInputMat(subInput32F3_0);
	sub32FC3.setInputMat("subImage", subInput32F3_1);

	subInput32F3_0 = cv::Scalar(100.0, 50.0, 25.0);
	subInput32F3_1 = cv::Scalar(200.0, 100.0, 50.0);
	sub32FC3.execute();
	output = sub32FC3.getOutputMat();
	ch = output.channels();
	test("SubMat output image rows equals to input",
		(output.rows == subInput32F3_0.rows), counter);
	test("SubMat output image cols equals to input",
		(output.cols == subInput32F3_0.cols), counter);
	test("SubMat output image type() equals to input",
		(output.type() == subInput32F3_0.type()), counter);
	test("SubMat output image channels() equals to input",
		(output.channels() == subInput32F3_0.channels()), counter);

	test("SubMat result value(0,0)", testPixel<float>(output, 0, 0, 0, -100.0), counter);
	test("SubMat result value(1,1)", testPixel<float>(output, 1, 1, 1, -50.0), counter);
	test("SubMat result value(2,2)", testPixel<float>(output, 2, 2, 2, -25.0), counter);

	subInput32F3_0 = cv::Scalar(200.0, 100.0, 50.0);
	subInput32F3_1 = cv::Scalar(100.0, 50.0, 25.0);
	sub32FC3.execute();
	output = sub32FC3.getOutputMat();
	test("SubMat result value(0,0)", testPixel<float>(output, 0, 0, 0, 100.0), counter);
	test("SubMat result value(1,1)", testPixel<float>(output, 1, 1, 1, 50.0), counter);
	test("SubMat result value(2,2)", testPixel<float>(output, 2, 2, 2, 25.0), counter);

	// test auto bind
	cv::Mat autoBindInpMat(cv::Mat::zeros(8,8,CV_8UC1));
	ImgProcSet proc;
	proc.setInputMat(autoBindInpMat);
	proc.add("ImagePoint", "p0");
	proc.add("ImagePoint", "p1");
	const cv::Mat& autoBindOutMat = proc.getInputMat();

	proc.execute();
	test("ImgProcSet auto bind #1.", testPixel<unsigned char>(autoBindOutMat, 0, 0, 0, 0), counter);
	
	autoBindInpMat = cv::Scalar(1);
	proc.execute();
	test("ImgProcSet auto bind #2.", testPixel<unsigned char>(autoBindOutMat, 0, 0, 0, 1), counter);

	autoBindInpMat = cv::Scalar(255);
	proc.execute();
	test("ImgProcSet auto bind #3.", testPixel<unsigned char>(autoBindOutMat, 0, 0, 0, 255), counter);

	// test auto bind
	cv::Mat lastAutoBindInpMat(cv::Mat::zeros(8,8,CV_8UC1));
	cv::Mat lastAutoBindInp2Mat(cv::Mat::zeros(8,8,CV_8UC1));
	ImgProcSet procLastAutoBind;
	procLastAutoBind.setInputMat(lastAutoBindInpMat);
	procLastAutoBind.add("ImagePoint", "input");
	procLastAutoBind.add("ImagePoint", "auto");
	procLastAutoBind.add("ImagePoint", "not_auto", false).setInputMat(lastAutoBindInp2Mat);
	//const cv::Mat& lastAutoBindOutMat = procLastAutoBind.getOutputMat();
	cv::Mat lastAutoBindOutMat;
	lastAutoBindInpMat = cv::Scalar(128);
	lastAutoBindInp2Mat = cv::Scalar(64);
	procLastAutoBind.execute();
	lastAutoBindOutMat = procLastAutoBind.getOutputMat();
	test("ImgProcSet output is last auto bind #1.", testPixel(lastAutoBindOutMat, 0, 0, 0, (unsigned char)128), counter);
	lastAutoBindInpMat = cv::Scalar(1);
	lastAutoBindInp2Mat = cv::Scalar(255);
	procLastAutoBind.execute();
	lastAutoBindOutMat = procLastAutoBind.getOutputMat();
	test("ImgProcSet output is last auto bind #2.", testPixel(lastAutoBindOutMat, 0, 0, 0, (unsigned char)1), counter);
	return 0;
}
