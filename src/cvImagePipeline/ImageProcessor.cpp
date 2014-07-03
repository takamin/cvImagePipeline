#include "ImageProcessor.h"
#include <typeinfo>
#include <time.h>
#include "pugixml-1.4/src/pugixml.hpp" // for ImgProcSet::loadXml
using namespace std;

namespace cvUtils {
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
								<< "を生成しました。" << std::endl;
						} else {
							std::cout << "ImageProcessor::createFilter("
								<< name << ") 生成失敗。ファクトリがnullを返却"
								<< std::endl;
						}
#endif
						return filter;
					} else {
#ifdef _DEBUG
						std::cout << "ImageProcessor::createFilter("
							<< name << ") 生成失敗。ファクトリがnull."
							<< std::endl;
#endif
					}
				}
			}
#ifdef _DEBUG
			std::cout << "ImageProcessor::createFilter("
				<< name << ") 生成失敗。ファクトリが見つかりません。"
				<< std::endl;
#endif
			return 0;
		}

		void ImageProcessor::entryFilter(std::string name, FILTER_FACTORY creator) {
			if(Factory::table == 0) {
#ifdef _DEBUG
				std::cout << "ファクトリテーブル生成" << std::endl;
#endif
				Factory::table = new Factory[Factory::tableSize];
			}
			size_t n = Factory::tableSize;
			for(size_t i = 0; i < n; i++) {
				Factory& entry = Factory::table[i];
				if(entry.name == name) {
#ifdef _DEBUG
					std::cout << "[" << i << "] ImageProcessor::entryFilter("
						<< name << ") 登録失敗。名前が重複。" << std::endl;
#endif
					return;
				} else if(entry.name == "" && entry.creator == 0) {
					entry.name = name;
					entry.creator = creator;
#ifdef _DEBUG
					std::cout << "[" << i << "] ImageProcessor::entryFilter("
						<< name << ") ファクトリ登録" << std::endl;
#endif
					return;
				}
			}
#ifdef _DEBUG
			std::cout << "ImageProcessor::entryFilter("
				<< name << ") 登録失敗。テーブルいっぱい" << std::endl;
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
		// class ImgProcSet

		IMPLEMENT_CVFILTER(ImgProcSet);

		ImgProcSet::ImgProcSet()
		{
		}
		ImgProcSet::~ImgProcSet()
		{
			std::vector<ImageProcessor*>::iterator dyn_filter = dyn_filters.begin();
			while(dyn_filter != dyn_filters.end()) {
#ifdef _DEBUG
				std::cout << "delete filter "
					<< typeid(**dyn_filter).name() << std::endl;
#endif
				delete *dyn_filter;
				*dyn_filter = 0;
				dyn_filter++;
			}
		}

		ImageProcessor& ImgProcSet::add(
			std::string filter_class_name,
			std::string filter_instance_name,
			bool autoBind)
		{
			ImageProcessor* filter = ImageProcessor::createFilter(filter_class_name);
			if(filter != 0) {
				dyn_filters.push_back(filter);
				name_to_filter[filter_instance_name] = filter;
				add(*filter, autoBind);
			}
			return *filter;
		}
		ImageProcessor& ImgProcSet::add(ImageProcessor& proc, bool autoBind)
		{
			if(autoBind) {
				std::list<ImageProcessor*>::reverse_iterator back = procs.rbegin();
				if(back != procs.rend()) {
					ImageProcessor* last = *back;
					proc >> *last;
					//proc.setInputMat(last->getOutputMat());
				} else {
					proc >> *this;
					//proc.setInputMat(getInputMat());
				}
				refOutputMat() = proc.getOutputMat();
			}
			procs.push_back(&proc);
			return proc;
		}
		void ImgProcSet::setParam(
			std::string filter_instance_name,
			std::string parameter_name, int value)
		{
			ImageProcessor* filter = name_to_filter[filter_instance_name];
			if(filter == 0) {
#ifdef _DEBUG
				std::cout << filter_instance_name
					<< "という名前のフィルターがない。"
					<< std::endl;
#endif
			} else {
				filter->property(parameter_name, value);
				//filter->setParam(parameter_name, value);
			}
		}
		ImageProcessor& ImgProcSet::operator[](
			std::string filter_instance_name)
		{
			ImageProcessor* filter = name_to_filter[filter_instance_name];
			return *filter;
		}
		
		void ImgProcSet::execute()
		{
			std::list<ImageProcessor*>::iterator proc = procs.begin();
			while(proc != procs.end()) {
				(*proc)->execute();
				proc++;
			}
			std::list<ImageProcessor*>::reverse_iterator back = procs.rbegin();
			if(back != procs.rend()) {
				ImageProcessor* last = *back;
				setOutputMat(last->getOutputMat());
			}
		}

		/**
		 * create procesor pipeline from xml file. 
		 */
		bool ImgProcSet::loadXml(const std::string& filename) {
			using namespace pugi;
			xml_document document;
			xml_parse_result result = document.load_file(filename.c_str());
			if(result.status != status_ok) {
				return false;
			}
			xml_node root_node = document.document_element();
#ifdef _DEBUG
			cerr << "[ImgProcSet::loadXml]" << "root node name = " << root_node.name() << endl;
#endif
			xpath_node_set processor_node_set = root_node.select_nodes("Processor");
			xpath_node_set::const_iterator processor = processor_node_set.begin();
			while(processor != processor_node_set.end()) {
				xpath_node_set property_node_set = processor->node().select_nodes("Property");
				xml_attribute attr_proc_class = processor->node().attribute("class");
				xml_attribute attr_proc_name = processor->node().attribute("name");
#ifdef _DEBUG
				cerr << "[ImgProcSet::loadXml]  " << "Processor "
					<< attr_proc_class.name() << " = " << attr_proc_class.as_string() << ", "
					<< attr_proc_name.name() << " = " << attr_proc_name.as_string() << endl;
#endif
				ImageProcessor& proc = add(attr_proc_class.as_string(), attr_proc_name.as_string());
				xpath_node_set::const_iterator property = property_node_set.begin();
				while(property != property_node_set.end()) {
					xml_attribute attr_prop_name = property->node().attribute("name");
					xml_attribute attr_prop_value = property->node().attribute("value");
#ifdef _DEBUG
					cerr << "[ImgProcSet::loadXml]    " << "Property "
						<< attr_prop_name.name() << " = " << attr_prop_name.as_string() << ", "
						<< attr_prop_value.name() << " = " << attr_prop_value.as_string() << endl;
#endif
					proc.setPropertyAsString(attr_prop_name.as_string(), attr_prop_value.as_string());
					property++;
				}
				processor++;
			}
			return true;
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
			cv::Mat mat;
			while(frameNumber < startFrame) {
				videoCapture >> mat;
				frameNumber++;
			}
			videoCapture >> mat;
			if(frameNumber >= startFrame && (stopFrame < startFrame || frameNumber <= stopFrame)) {
				setOutputMat(mat.clone());
			} else {
				cv::Mat zero;
				setOutputMat(zero);
			}
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
					//std::cout << ss.str() << std::endl;
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
