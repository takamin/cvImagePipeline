struct test_counter {
	test_counter() : test_count(0), pass_count(0), fail_count(0) {}
	void test(const string& name, bool result);
	void testMatDepth(const string& name, const cv::Mat& mat, int depth);
	int test_count;
	int pass_count;
	int fail_count;
	static int types[];
	static cv::Mat mat[];
	static int typesCount();
	static int matCount();
};

template<class T> bool is_equal(const T& a, const T& b)
{
	return (a == b);
}
bool is_equal(const float& a, const float& b);
bool is_equal(const double& a, const double& b);
template<class PixelT>bool testPixel(const cv::Mat& image, int x, int y, int ch, PixelT test_value) {
	PixelT* p = (PixelT*)(image.data + image.step * y);
	p += x * image.channels();
	if (is_equal(p[ch], test_value)) {
		return true;
	}
	cerr << "test fail: expected " << test_value << " at (" << x << ", " << y << ", " << ch << "), but " << p[ch] << std::endl;
	return false;
}
bool testPixel(const cv::Mat& image, int x, int y, int ch, unsigned char test_value);
bool testPixel(const cv::Mat& image, int x, int y, int ch, char test_value);

bool tryExecute(ImageProcessor* proc);
const char* depth_string(int depth);
string mat_toString(const cv::Mat& mat);
bool testProcessor(ImageProcessor* proc);
