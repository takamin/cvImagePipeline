#include "ImageProcessor.h"
#include <typeinfo>
#include <time.h>
#include <sstream>
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
		bool PropSet::exists(const std::string& name) const {
			return namedValueMap.count(name) > 0;
		}

		/////////////////////////////////////////////////////////////////////
		//ImageProcessor
		int ImageProcessor::instanceNumber = 0;
		ImageProcessor::ImageProcessor() {
			inputMat[""] = 0;
			
			// set name;
			stringstream ss;
			ss << typeid(*this).name() << "#" << ImageProcessor::instanceNumber;
			name = ss.str();

			ImageProcessor::instanceNumber++;
		}

		ImageProcessor::~ImageProcessor()
		{
		}

		void ImageProcessor::reset() 
		{
		}

		void ImageProcessor::defInputMat(const std::string& name)
		{
			inputMat[name] = 0;
		}
		const cv::Mat& ImageProcessor::getInputMat() const {
			checkInputMatExists("");
			return *((ImageProcessor*)this)->inputMat[""];
		}
		const cv::Mat& ImageProcessor::getInputMat(const std::string& name) const
		{
			checkInputMatExists(name);
			return *((ImageProcessor*)this)->inputMat[name];
		}
		void ImageProcessor::defParam(Property& param) {
			parameters.add(&param);
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
			const std::string& name,
			const cv::Mat& mat)
		{
			checkInputMatExists(name);
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
			checkPropertyExists(parameter_name);
			parameters[parameter_name].setString(value);
			onPropertyChange(parameters[parameter_name]);
			return *this;
		}
		const Property& ImageProcessor::property(
			const std::string& parameter_name) const
		{
			checkPropertyExists(parameter_name);
			return parameters[parameter_name];
		}

		void ImageProcessor::onPropertyChange(Property& property) {
#ifdef _DEBUG
			std::cout << "ImageProcessor::onPropertyChange("
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
		
		bool ImageProcessor::checkPropertyExists(const std::string& name) const
		{
			if(!parameters.exists(name)) {
				std::cerr << "property \"" << name << "\" not exists in "
					<< name << " instance of class " << std::string(typeid(*this).name()) << "."
					<< std::endl;
				throw new std::exception("PropertyNotFound");
			}
			return true;
		}
		bool ImageProcessor::checkInputMatExists(const std::string& name) const
		{
			if(inputMat.count(name) <= 0) {
				std::cerr << "input mat \"" << name << "\" not exists in "
					<< name << " instance of class " << std::string(typeid(*this).name()) << "."
					<< std::endl;
				throw new std::exception("InputMatNotFound");
			}
			return true;
		}

		void ImageProcessor::putMarkdown(std::ostream& stream) {
			stream << "# " << typeid(*this).name() << std::endl;
			stream << std::endl;
			stream << "## Input " << std::endl;
			stream << std::endl;
			
			MapMat::iterator mapEntry =  inputMat.begin();
			for(; mapEntry != inputMat.end(); mapEntry++) {
				MapMat::key_type name = mapEntry->first;
				MapMat::mapped_type pMap = mapEntry->second;
				if(name == "") {
					stream << "* (standard)" << std::endl;
				} else {
					stream << "* " << name << std::endl;
				}
			}
			stream << std::endl;

			stream << "## Property" << std::endl;
			stream << std::endl;
			const PropList& properties = parameters.getPropertyList();
			PropList::const_iterator p = properties.begin();
			for(; p != properties.end(); p++) {
				Property* property = *p;
				stream << "* " << property->getName() << " = "
					<< property->getString() << std::endl;
			}
			stream << std::endl;
		}
	}
}
