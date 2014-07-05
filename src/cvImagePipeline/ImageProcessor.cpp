#include "ImageProcessor.h"
#include <typeinfo>
#include <time.h>
using namespace std;
namespace cvImagePipeline {
	namespace Filter {
		
		/////////////////////////////////////////////////////////////////////
		//PropSet
		void PropSet::add(Property* p) {
			std::string name = p->getName();
			namedValueMap.insert(PropNameMap::value_type(name, p));
			namedValueList.push_back(p);
		}
		Property& PropSet::operator[](const std::string& name) {
			return *(namedValueMap.at(name));
		}
		const Property& PropSet::operator[](const std::string& name) const {
			return *(namedValueMap.at(name));
		}

		/////////////////////////////////////////////////////////////////////
		//ImageProcessor
		
		ImageProcessor::ImageProcessor() {
			inputMat[""] = 0;
		}

		ImageProcessor::~ImageProcessor()
		{
		}

		void ImageProcessor::reset() 
		{
		}

		void ImageProcessor::defInputMat(std::string name)
		{
			inputMat[name] = 0;
		}
		const cv::Mat& ImageProcessor::getInputMat(std::string name) const
		{
			return *((ImageProcessor*)this)->inputMat[name];
		}

		void ImageProcessor::defParam(Property& param) {
			parameters.add(&param);
		}

		ImageProcessor&
		ImageProcessor::operator << (const ImageProcessor& src) {
			setInputMat(src.getOutputMat());
			return *this;
		}

		ImageProcessor&
		ImageProcessor::operator >> (ImageProcessor& dst) {
			dst.setInputMat(getOutputMat());
			onOutputMatConnectedTo(dst, "");
			return dst;
		}

		ImageProcessor::FilterInput
		ImageProcessor::input(std::string name) {
			ImageProcessor::FilterInput mnp;
			mnp.filter = this;
			mnp.name = name;
			return mnp;
		}

		ImageProcessor&
		ImageProcessor::operator >> (
			ImageProcessor::FilterInput& mnp)
		{
			mnp.filter->setInputMat(mnp.name, getOutputMat());
			onOutputMatConnectedTo(*mnp.filter, mnp.name);
			return *this;
		}

		void ImageProcessor::setInputMat(const cv::Mat& mat) {
			inputMat[""] = &mat;
			onInputMatConnected("");
		}

		void ImageProcessor::setInputMat(
			std::string name,
			const cv::Mat& mat)
		{
			inputMat[name] = &mat;
			onInputMatConnected(name);
		}

		void ImageProcessor::setOutputMat(const cv::Mat& mat) {
			outputMat = mat;
		}

		ImageProcessor& ImageProcessor::setPropertyAsString(
			const std::string& parameter_name,
			const std::string& value)
		{
			parameters[parameter_name].setString(value);
			onPropetyChange(parameters[parameter_name]);
			return *this;
		}

		void ImageProcessor::onPropetyChange(Property& property) {
#ifdef _DEBUG
			std::cout << "ImageProcessor::onPropetyChange("
				<< "class " << typeid(property).name() << " "
				<< property.getName() << " = \""
				<< property.getString() << "\")"
				<< std::endl;
#endif
		}
		void ImageProcessor::onInputMatConnected(const std::string& inputMatName) {
#ifdef _DEBUG
			std::cout << "ImageProcessor::onInputMatConnected(\""
				<< inputMatName << "\")" << std::endl;
#endif
		}
		void ImageProcessor::onOutputMatConnectedTo(
			ImageProcessor& dst,
			const std::string& inputMatName)
		{
#ifdef _DEBUG
			std::cout << "ImageProcessor::onOutputMatConnectedTo("
				<< "class " << typeid(dst).name() << " inputMatName=\"" << inputMatName << "\")" << std::endl;
#endif
		}

		/////////////////////////////////////////////////////////////////////
		// Implement ImageProcessor factory by its class name.
		
		size_t ImageProcessor::Factory::tableSize = 100;
		ImageProcessor::Factory* ImageProcessor::Factory::table = 0;
		ImageProcessor* ImageProcessor::createFilter(std::string name) {
			size_t n = Factory::tableSize;
			for(size_t i = 0; i < n; i++) {
				Factory& entry = Factory::table[i];
				if(entry.name == name) {
					if(entry.creator != 0) {
						ImageProcessor* filter = (*entry.creator)();
#ifdef _DEBUG
						if(filter != 0) {
							std::cout << "ImageProcessor::createFilter("
								<< name << ") " << typeid(*filter).name()
								<< "�𐶐����܂����B" << std::endl;
						} else {
							std::cout << "ImageProcessor::createFilter("
								<< name << ") �������s�B�t�@�N�g����null��ԋp"
								<< std::endl;
						}
#endif
						return filter;
					} else {
#ifdef _DEBUG
						std::cout << "ImageProcessor::createFilter("
							<< name << ") �������s�B�t�@�N�g����null."
							<< std::endl;
#endif
					}
				}
			}
#ifdef _DEBUG
			std::cout << "ImageProcessor::createFilter("
				<< name << ") �������s�B�t�@�N�g����������܂���B"
				<< std::endl;
#endif
			return 0;
		}

		void ImageProcessor::entryFilter(std::string name, FILTER_FACTORY creator) {
			if(Factory::table == 0) {
#ifdef _DEBUG
				std::cout << "�t�@�N�g���e�[�u������" << std::endl;
#endif
				Factory::table = new Factory[Factory::tableSize];
			}
			size_t n = Factory::tableSize;
			for(size_t i = 0; i < n; i++) {
				Factory& entry = Factory::table[i];
				if(entry.name == name) {
#ifdef _DEBUG
					std::cout << "[" << i << "] ImageProcessor::entryFilter("
						<< name << ") �o�^���s�B���O���d���B" << std::endl;
#endif
					return;
				} else if(entry.name == "" && entry.creator == 0) {
					entry.name = name;
					entry.creator = creator;
#ifdef _DEBUG
					std::cout << "[" << i << "] ImageProcessor::entryFilter("
						<< name << ") �t�@�N�g���o�^" << std::endl;
#endif
					return;
				}
			}
#ifdef _DEBUG
			std::cout << "ImageProcessor::entryFilter("
				<< name << ") �o�^���s�B�e�[�u�������ς�" << std::endl;
#endif
		}

		///////////////////////////////////////////////////////////////////////////
		// class ImagePoint
		// always outputs the input image

		IMPLEMENT_CVFILTER(ImagePoint);
		void ImagePoint::execute(){
			refOutputMat() = getInputMat();
		}

		///////////////////////////////////////////////////////////////////////////
		// class VideoCapture

		IMPLEMENT_CVFILTER(VideoCapture);
		VideoCapture::VideoCapture()
		:	
			deviceIndex("deviceIndex", -1), filename("filename", ""),
			startFrame("startFrame", 0), stopFrame("stopFrame", -1),
			width("width", 0.0),
			height("height", 0.0),
			frameNumber(0)
		{
			defParam(deviceIndex);
			defParam(filename);
			defParam(startFrame);
			defParam(stopFrame);
			defParam(width);
			defParam(height);
		}
		VideoCapture::~VideoCapture() { }
		bool VideoCapture::open(int deviceIndex)
		{
			bool open_state = videoCapture.open(deviceIndex);

			if(width != 0.0) {
				videoCapture.set(CV_CAP_PROP_FRAME_WIDTH, width);
			}
			if(height != 0.0) {
				videoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, height);
			}
			return open_state;
		}
		bool VideoCapture::open(const std::string& filename)
		{ 
			bool open_state = videoCapture.open(filename);
			if(width != 0.0) {
				videoCapture.set(CV_CAP_PROP_FRAME_WIDTH, width);
			}
			if(height != 0.0) {
				videoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, height);
			}
			return open_state;
		}
		bool VideoCapture::capture() {
			if(!videoCapture.isOpened()) {
				if(deviceIndex >= 0) {
					open(deviceIndex);
				} else if((string)filename != "") {
					open((string)filename);
				} else {
					return false;
				}
			}
			cv::Mat& mat = refOutputMat(); 
			while(frameNumber < startFrame) {
				videoCapture >> mat;
				frameNumber++;
			}
			videoCapture >> mat;
			frameNumber++;
			return true;
		}
		void VideoCapture::execute() {
			capture();
		}

		///////////////////////////////////////////////////////////////////////////
		// class ImageWindow

		IMPLEMENT_CVFILTER(ImageWindow);
		ImageWindow::ImageWindow()
			: windowName("windowName", string("cv::Mat")),
			showFrameNumber("showFrameNumber", false), showFPS("showFPS", false),
			frameNumber(0), tick(0), windowCreated(false)
		{
			defParam(windowName);
			defParam(showFrameNumber);
			defParam(showFPS);
		}
		ImageWindow::ImageWindow(std::string windowName)
			: windowName("windowName", windowName),
			showFrameNumber("showFrameNumber", false), showFPS("showFPS", false),
			frameNumber(0), tick(0), windowCreated(true)
		{
			cvNamedWindow(windowName.c_str(), CV_WINDOW_AUTOSIZE);
			defParam(showFrameNumber);
			defParam(showFPS);
		}
		void ImageWindow::execute() {
			if(!windowCreated) {
				cvNamedWindow(((string)windowName).c_str(), CV_WINDOW_AUTOSIZE);
				windowCreated = true;
			}
			const cv::Mat& image = getInputMat();
			long curr_tick = clock();
			if(!image.empty()) {
				cv::Mat im2show = image.clone();
				if(showFrameNumber || showFPS) {
					std::stringstream ss;
					if(showFrameNumber) {
						ss << "[#" << frameNumber << "] ";
					}
					if(showFPS) {
						double fps = (double)CLOCKS_PER_SEC / (curr_tick - tick); 
						ss << std::fixed << fps << "[fps] ";
					}
					cv::putText(im2show, ss.str(), cv::Point(15, 15),
						cv::FONT_HERSHEY_COMPLEX_SMALL, 0.5,
						cv::Scalar(0,255,0), 1); 
				}
				cv::imshow(windowName, im2show);
				setOutputMat(image.clone());
			}
			tick = curr_tick;
			frameNumber++;
		}

		///////////////////////////////////////////////////////////////////////////
		// class ColorConverter

		IMPLEMENT_CVFILTER(ColorConverter);
		ColorConverter::ColorConverter()
			: cvtCode("cvtCode", CV_BGR2GRAY)
			{ defParam(this->cvtCode); }
		ColorConverter::ColorConverter(int cvtCode)
			: cvtCode("cvtCode", cvtCode)
			{ defParam(this->cvtCode); }
		void ColorConverter::execute() {
			const cv::Mat& inputMat = getInputMat();
			if(!inputMat.empty()) {
				cv::cvtColor(inputMat, refOutputMat(), cvtCode);
			}
		}

		///////////////////////////////////////////////////////////////////////////
		// class Resizer

		IMPLEMENT_CVFILTER(Resizer);
		Resizer::Resizer()
			: width("width", 640), height("height", 480)
		{
			defParam(this->width);
			defParam(this->height);
		}
		Resizer::Resizer(int width, int height)
			: width("width", width), height("height", height)
		{
			defParam(this->width);
			defParam(this->height);
		}
		void Resizer::execute() {
			const cv::Mat& inputMat = getInputMat();
			if(!inputMat.empty()) {
				cv::resize(inputMat, refOutputMat(), cv::Size(width, height));
			}
		}

		///////////////////////////////////////////////////////////////////////////
		// class Flipper

		IMPLEMENT_CVFILTER(Flipper);
		Flipper::Flipper()
			: flipDir("flipDir", 1)
			{ defParam(this->flipDir); }
		Flipper::Flipper(int flipDir)
			: flipDir("flipDir", flipDir)
			{ defParam(this->flipDir); }
		void Flipper::execute() {
			const cv::Mat& inputMat = getInputMat();
			if(!inputMat.empty()) {
				cv::flip(inputMat, refOutputMat(), flipDir);
			}
		}

		///////////////////////////////////////////////////////////////////////////
		// class GaussianBlur

		IMPLEMENT_CVFILTER(GaussianBlur);
		GaussianBlur::GaussianBlur()
			:	kernelWidth("kernelWidth", 5),
				kernelHeight("kernelHeight", 5),
				sigmaX("sigmaX", 1.0),
				sigmaY("sigmaY", 0.0),
				borderType("borderType", cv::BORDER_DEFAULT)
		{
			defParam(this->kernelWidth);
			defParam(this->kernelHeight);
			defParam(this->sigmaX);
			defParam(this->sigmaY);
			defParam(this->borderType);
		}
		GaussianBlur::GaussianBlur(
			int kernelWidth, int kernelHeight, 
			double sigmaX, double sigmaY,
			int borderType)
			:	kernelWidth("kernelWidth", kernelWidth),
				kernelHeight("kernelHeight", kernelHeight),
				sigmaX("sigmaX", sigmaX),
				sigmaY("sigmaY", sigmaY),
				borderType("borderType", borderType)
		{
			defParam(this->kernelWidth);
			defParam(this->kernelHeight);
			defParam(this->sigmaX);
			defParam(this->sigmaY);
			defParam(this->borderType);
		}
		void GaussianBlur::execute() {
			const cv::Mat& inputMat = getInputMat();
			cv::GaussianBlur(
				inputMat, refOutputMat(),
				cv::Size(kernelWidth, kernelHeight),
				sigmaX, sigmaY, borderType);
		}
	}
}
