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
			onPropertyChange(parameters[parameter_name]);
			return *this;
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
	}
}
