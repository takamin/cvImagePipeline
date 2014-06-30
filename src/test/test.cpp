// test.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "ImageProcessor.h"

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
		(false == imgProcSet.loadXml("..\\test\\ReadMe.txt")),
		counter);

	test("imgProcSet.loadXml returns true certain xml file",
		(true == imgProcSet.loadXml("..\\test\\certain.xml")),
		counter);
	while(true) {
		imgProcSet.execute();
		if(cvWaitKey(1) == '\x1b') {
			break;
		}
	}
	return 0;
}

