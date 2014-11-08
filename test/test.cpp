// test.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "test.h"

int test_counter::types[] = {
	CV_8UC1, CV_8UC3,
	CV_16UC1, CV_16UC3,
	CV_32FC1, CV_32FC3,
	CV_64FC1, CV_64FC3,
};
cv::Mat test_counter::mat[] = {
	cv::Mat::zeros(480, 640, CV_8UC1),
	cv::Mat::zeros(480, 640, CV_8UC3),
	cv::Mat::zeros(480, 640, CV_16SC1),
	cv::Mat::zeros(480, 640, CV_16SC3),
	cv::Mat::zeros(480, 640, CV_32FC1),
	cv::Mat::zeros(480, 640, CV_32FC3),
	cv::Mat::zeros(480, 640, CV_64FC1),
	cv::Mat::zeros(480, 640, CV_64FC3),
};
int test_counter::typesCount() {
	return sizeof(test_counter::types) / sizeof(test_counter::types[0]);
}
int test_counter::matCount() {
	return sizeof(test_counter::mat) / sizeof(test_counter::mat[0]);
}

void test_counter::test(const string& name, bool result) {
	string result_str;
	test_count++;
	if(result) {
		pass_count++;
	} else {
		fail_count++;
		cerr << "[FAIL] test " << test_count << ". " << name << endl;
	}
}
void test_counter::testMatDepth(const string& name, const cv::Mat& mat, int depth) {
	test(name, mat.depth() == (depth & CV_MAT_DEPTH_MASK));
}
bool is_equal(const float& a, const float& b) {
	return (abs(a - b) < FLT_EPSILON);
}
bool is_equal(const double& a, const double& b) {
	return (abs(a - b) < DBL_EPSILON);
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

bool tryExecute(ImageProcessor* proc) {
	bool result = true;
	try {
		proc->execute();
	}
	catch (cv::Exception& e) {
		std::cerr << "EXCEPTION: " << e.msg << std::endl;
		result = false;
	}
	return result;
}
const char* depth_string(int depth) {
	switch (depth) {
	case CV_8S: return "CV_8S";
	case CV_8U: return "CV_8U";
	case CV_16S: return "CV_16S";
	case CV_16U: return "CV_16U";
	case CV_32F: return "CV_32F";
	case CV_64F: return "CV_64F";
	}
	return "????";
}
string mat_toString(const cv::Mat& mat) {
	std::stringstream ss;
	ss << "cv::Mat(" << mat.cols << ", " << mat.rows << ", "
		<< depth_string(mat.depth()) << mat.channels() << ")";
	return ss.str();
}

bool testProcessor(ImageProcessor* proc) {
	std::cerr << "test " << proc->getName() << std::endl;
	bool status = true;
	for (int i = 0; i < sizeof(test_counter::mat) / sizeof(test_counter::mat[0]); i++) {
		std::cerr << "input mat " << mat_toString(test_counter::mat[i]) << std::endl;
		proc->setInputMat(test_counter::mat[i]);
		if (!tryExecute(proc)) {
			status = false;
		}
		std::cerr << "output mat " << mat_toString(proc->getOutputMat()) << std::endl;
	}
	return status;
}


void testRunningAvg(std::string processorClassName, test_counter& counter);
#if defined(_MSC_VER)
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif
{
	test_counter counter;

	ImgProcSet imgProcSet;
	counter.test("imgProcSet.loadXml returns false when file not exists.",
		(false == imgProcSet.loadXml("not_exists.xml")));

	counter.test("imgProcSet.loadXml returns false when no xml file",
		(false == imgProcSet.loadXml("..\\..\\test\\ReadMe.txt")));

	counter.test("imgProcSet.loadXml returns true certain xml file",
		(true == imgProcSet.loadXml("..\\..\\test\\certain.xml")));
	
	imgProcSet.putMarkdown(std::cerr);
	while(true) {
		imgProcSet.execute();
		if(cvWaitKey(1) == '\x1b') {
			break;
		}
	}


	/////////////////////////////////////////////////////////////////////////
	// SubMat 32FC3
	/////////////////////////////////////////////////////////////////////////

	SubMat sub32FC3;
	cv::Mat subInput32F3_0;
	cv::Mat subInput32F3_1;

	subInput32F3_0 = cv::Mat::zeros(3, 3, CV_32FC3);
	subInput32F3_1 = cv::Mat::zeros(3, 2, CV_32FC3);
	sub32FC3.setInputMat("src1", subInput32F3_0);
	sub32FC3.setInputMat("src2", subInput32F3_1);
	try { sub32FC3.execute(); } catch (...) { }
	cv::Mat output = sub32FC3.getOutputMat();
	counter.test("SubMat cannot calc between differ image. output image is empty()",
		(output.empty()));

	subInput32F3_0 = cv::Mat::zeros(3, 3, CV_32FC3);
	subInput32F3_1 = cv::Mat::zeros(3, 3, CV_32FC3);
	sub32FC3.setInputMat("src1", subInput32F3_0);
	sub32FC3.setInputMat("src2", subInput32F3_1);

	subInput32F3_0 = cv::Scalar(100.0, 50.0, 25.0);
	subInput32F3_1 = cv::Scalar(200.0, 100.0, 50.0);
	sub32FC3.execute();
	output = sub32FC3.getOutputMat();
	int ch = output.channels();
	counter.test("SubMat output image rows equals to input",
		(output.rows == subInput32F3_0.rows));
	counter.test("SubMat output image cols equals to input",
		(output.cols == subInput32F3_0.cols));
	counter.test("SubMat output image type() equals to input",
		(output.type() == subInput32F3_0.type()));
	counter.test("SubMat output image channels() equals to input",
		(output.channels() == subInput32F3_0.channels()));

	counter.test("SubMat result value(0,0)", testPixel<float>(output, 0, 0, 0, -100.0));
	counter.test("SubMat result value(1,1)", testPixel<float>(output, 1, 1, 1, -50.0));
	counter.test("SubMat result value(2,2)", testPixel<float>(output, 2, 2, 2, -25.0));

	subInput32F3_0 = cv::Scalar(200.0, 100.0, 50.0);
	subInput32F3_1 = cv::Scalar(100.0, 50.0, 25.0);
	sub32FC3.execute();
	output = sub32FC3.getOutputMat();
	counter.test("SubMat result value(0,0)", testPixel<float>(output, 0, 0, 0, 100.0));
	counter.test("SubMat result value(1,1)", testPixel<float>(output, 1, 1, 1, 50.0));
	counter.test("SubMat result value(2,2)", testPixel<float>(output, 2, 2, 2, 25.0));

	// test auto bind
	cv::Mat autoBindInpMat(cv::Mat::zeros(8,8,CV_8UC1));
	ImgProcSet proc;
	proc.setInputMat(autoBindInpMat);
	proc.add("ImagePoint", "p0");
	proc.add("ImagePoint", "p1");
	const cv::Mat& autoBindOutMat = proc.getInputMat();

	proc.execute();
	counter.test("ImgProcSet auto bind #1.", testPixel<unsigned char>(autoBindOutMat, 0, 0, 0, 0));
	
	autoBindInpMat = cv::Scalar(1);
	proc.execute();
	counter.test("ImgProcSet auto bind #2.", testPixel<unsigned char>(autoBindOutMat, 0, 0, 0, 1));

	autoBindInpMat = cv::Scalar(255);
	proc.execute();
	counter.test("ImgProcSet auto bind #3.", testPixel<unsigned char>(autoBindOutMat, 0, 0, 0, 255));

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
	counter.test("ImgProcSet output is last auto bind #1.", testPixel(lastAutoBindOutMat, 0, 0, 0, (unsigned char)128));
	lastAutoBindInpMat = cv::Scalar(1);
	lastAutoBindInp2Mat = cv::Scalar(255);
	procLastAutoBind.execute();
	lastAutoBindOutMat = procLastAutoBind.getOutputMat();
	counter.test("ImgProcSet output is last auto bind #2.", testPixel(lastAutoBindOutMat, 0, 0, 0, (unsigned char)1));


	/////////////////////////////////////////////////////////////////////////
	// RunningAvg
	/////////////////////////////////////////////////////////////////////////
	testRunningAvg("RunningAvg", counter);
	return 0;
}


void testRunningAvg(std::string processorClassName, test_counter& counter) {

	// 32FC3	
	
	ImageProcessor* pAvg = ImageProcessor::createFilter(processorClassName);
	ImageProcessor& averager32FC3 = *pAvg;
	averager32FC3.property("averageCount", 2);
	cv::Mat avgInput32F3_0(cv::Mat::zeros(3, 3, CV_32FC3));
	cv::Mat avgInput32F3_1(cv::Mat::zeros(3, 3, CV_32FC3));
	cv::Mat avgInput32F3_2(cv::Mat::zeros(3, 3, CV_32FC3));
	cv::Mat avgInput32F3_3(cv::Mat::zeros(3, 3, CV_32FC3));

	avgInput32F3_1 = cv::Scalar(100.0, 100.0, 100.0);
	avgInput32F3_2 = cv::Scalar(200.0, 200.0, 200.0);
	avgInput32F3_3 = cv::Scalar(300.0, 300.0, 300.0);

	averager32FC3.setInputMat(avgInput32F3_0);
	averager32FC3.execute();
	cv::Mat output = averager32FC3.getOutputMat();
	int ch = output.channels();
	counter.test(processorClassName + " output image rows equals to input",
		(output.rows == avgInput32F3_0.rows));
	counter.test(processorClassName + " output image cols equals to input",
		(output.cols == avgInput32F3_0.cols));
	counter.test(processorClassName + " output image type() equals to input",
		(output.type() == avgInput32F3_0.type()));
	counter.test(processorClassName + " output image channels() equals to input",
		(output.channels() == avgInput32F3_0.channels()));

	counter.test(processorClassName + " average[0] value(0,0)", testPixel<float>(output, 0, 0, 0, 0.0));
	counter.test(processorClassName + " average[0] value(1,1)", testPixel<float>(output, 1, 1, 1, 0.0));
	counter.test(processorClassName + " average[0] value(2,2)", testPixel<float>(output, 2, 2, 2, 0.0));
	
	averager32FC3.setInputMat(avgInput32F3_1);
	averager32FC3.execute();
	output = averager32FC3.getOutputMat();
	counter.test(processorClassName + " average[1] value(0,0)", testPixel<float>(output, 0, 0, 0, 50.0));
	counter.test(processorClassName + " average[1] value(1,1)", testPixel<float>(output, 1, 1, 1, 50.0));
	counter.test(processorClassName + " average[1] value(2,2)", testPixel<float>(output, 2, 2, 2, 50.0));

	averager32FC3.setInputMat(avgInput32F3_2);
	averager32FC3.execute();
	output = averager32FC3.getOutputMat();
	counter.test(processorClassName + " average[2] value(0,0)", testPixel<float>(output, 0, 0, 0, 125.0));
	counter.test(processorClassName + " average[2] value(1,1)", testPixel<float>(output, 1, 1, 1, 125.0));
	counter.test(processorClassName + " average[2] value(2,2)", testPixel<float>(output, 2, 2, 2, 125.0));

	averager32FC3.setInputMat(avgInput32F3_3);
	averager32FC3.execute();
	output = averager32FC3.getOutputMat();
	counter.test(processorClassName + " average[3] value(0,0)", testPixel<float>(output, 0, 0, 0, 212.5));
	counter.test(processorClassName + " average[3] value(1,1)", testPixel<float>(output, 1, 1, 1, 212.5));
	counter.test(processorClassName + " average[3] value(2,2)", testPixel<float>(output, 2, 2, 2, 212.5));

	delete pAvg;
}
