#pragma once
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "../source/pugixml-1.4/src/pugixml.hpp"

#pragma warning(disable:4251)

#if defined(_MSC_VER)
    //  Microsoft 
    #define EXPORT __declspec(dllexport)
    #define IMPORT __declspec(dllimport)
#elif defined(_GCC)
    //  GCC
    #define EXPORT __attribute__((visibility("default")))
    #define IMPORT
#else
    //  do nothing and hope for the best?
    #define EXPORT
    #define IMPORT
    #pragma warning Unknown dynamic link import/export semantics.
#endif
#define SHARED EXPORT

namespace cvImagePipeline {
	namespace Filter {

		//property for ImageProcessor
		class SHARED Property {
			std::string name;
		protected:
			Property(std::string name) : name(name) {}
		public:
			const std::string& getName() const { return name; }
			virtual std::string getString() const = 0;
			virtual void setString(const std::string& s) = 0;
		};

		//property set
		typedef std::map<std::string, Property*> PropNameMap;
		typedef std::list<Property*> PropList;
		class SHARED PropSet
		{
			PropNameMap namedValueMap;
			PropList namedValueList;
		public:
			void add(Property* p);
			Property& operator[](const std::string& name);
			const Property& operator[](const std::string& name) const;
			template<class T> void set(const std::string& name, T value) {
				std::stringstream ss;
				ss << value;
				cvImagePipeline::Filter::Property* p = namedValueMap.at(name);
				if (p) {
					p->setString(ss.str());
				}
			}
			void set(const std::string& name, const std::string& value) {
				cvImagePipeline::Filter::Property* p = namedValueMap.at(name);
				if (p) {
					p->setString(value);
				}
			}
			bool exists(const std::string& name) const;
			const PropList& getPropertyList() const { return namedValueList; }
		};

		//primitive property template
		template<class ValueType> class SHARED TParam : public Property
		{
			ValueType value;
		public:
			TParam(std::string name, ValueType value) : Property(name), value(value) { }
			ValueType getValue() const { return value; }
			void setValue(ValueType value) { this->value = value; }
			operator ValueType () const { return value; }
			TParam& operator = (ValueType value) {
				this->value = value;
				return *this;
			}
		public:
			std::string getString() const {
				std::stringstream ss;
				ss << value;
				return ss.str();
			}
			void setString(const std::string& s) {
				std::istringstream is(s);
				is >> value;
			}
		};

		//ImageProcessorの派生クラスのインスタンスをクラス名から生成するための定義
		class ImageProcessor;
		typedef ImageProcessor*(*FILTER_FACTORY)();
		#define DECLARE_CVFILTER \
			static ImageProcessor* createFilter();\
			static void entryFilter()
		#define IMPLEMENT_CVFILTER(class_name) \
			cvImagePipeline::Filter::ImageProcessor* class_name::createFilter() { return new class_name; } \
			void class_name::entryFilter() { cvImagePipeline::Filter::ImageProcessor::entryFilter(#class_name, &class_name::createFilter); } \
			cvImagePipeline::Filter::InitialCallback<class_name, class_name::entryFilter> class_name ## _entry_static_
		#define FORCE_LINK_CVFILTER(class_name) \
			static FILTER_FACTORY class_name ## _createFilter = class_name::createFilter

		template<class T, void func()> class InitialCallback { public: InitialCallback() { func(); } };
		
		//イメージプロセッサ
		class SHARED ImageProcessor {
		public:
			struct FilterInput {
				ImageProcessor* filter;
				std::string name;
			};
		protected:
			void setDescription(const std::string& description);
			void defInputMat(const std::string& name);
			void defInputMat(const std::string& name, const std::string& description);
			void setInputMatDesc(const std::string& name, const std::string& description);
			void undefInputMat(const std::string& name);
			void defParam(Property& param);
			void setOutputMat(const cv::Mat& mat);
			cv::Mat& refOutputMat() { return outputMat; }
		public:
			ImageProcessor();
			virtual ~ImageProcessor();
			const std::string& getName() const { return name; }
			void setName(const std::string& name) { this->name = name; }
			const std::string& getDescription() const { return description; }
			virtual void reset();
			virtual void onPropertyChange(Property& property);
			virtual void onInputMatConnected(const std::string& inputMatName);
			virtual void onOutputMatConnectedTo(ImageProcessor& dst, const std::string& inputMatName);
			template<class T> ImageProcessor& property(const std::string& parameter_name, T value) {
				checkPropertyExists(parameter_name);
				parameters.set(parameter_name, value);
				onPropertyChange(parameters[parameter_name]);
				return *this;
			}
			ImageProcessor& setPropertyAsString(
				const std::string& parameter_name, const std::string& value);
			const Property& property(const std::string& parameter_name) const;
			
			ImageProcessor& setInputMat(const cv::Mat& mat);
			ImageProcessor& setInputMat(const std::string& name, const cv::Mat& mat);
			const cv::Mat& getInputMat() const;
			const cv::Mat& getInputMat(const std::string& name) const;
			const cv::Mat& getOutputMat() const { return outputMat; }

			virtual void execute() = 0;

			ImageProcessor& operator >> (ImageProcessor& dst);
			FilterInput input(std::string name);
			ImageProcessor& operator >> (FilterInput& dst);
			void setPropertyByXmlNode(pugi::xml_node property);
			virtual void putMarkdown(std::ostream& stream) const;
		private:
			typedef std::map<std::string, const cv::Mat*> MapMat;
			std::string name;
			std::string description;
			PropSet parameters;
			MapMat inputMat;
			std::map<std::string, std::string> inputMatDescription;
			cv::Mat outputMat;
			struct Factory {
				std::string name;
				FILTER_FACTORY creator;
				Factory() : name(""),creator(0) {}
				static size_t tableSize;
				static Factory* table;
			};
		private:
			bool checkInputMatExists(const std::string& name) const;
			bool checkPropertyExists(const std::string& name) const;

		public:
			static int instanceNumber;
			static ImageProcessor* createFilter(std::string name);
			static void entryFilter(std::string name, FILTER_FACTORY creator);

			//処理の有効化フラグ。効果の有無はサブクラスの実装依存
		private:
			bool enable;//コンストラクタでtrueに初期化。
		public:
			//設定
			virtual void setEnable(bool enable) { this->enable = enable; }
			//取得
			bool isEnable() const { return enable; }
		};
	}
}
#pragma warning(default:4251)
