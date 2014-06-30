#pragma once

#include <hash_map>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>

#include <opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#pragma warning(disable:4251)


namespace cvUtils {

	namespace Filter {

		//property for ImageProcessor
		class __declspec(dllexport) Property {
			std::string name;
		protected:
			Property(std::string name) : name(name) {}
		public:
			std::string getName() const { return name; }
			virtual std::string getString() const = 0;
			virtual void setString(const std::string& s) = 0;
		};

		//property set
		typedef std::map<std::string, Property*> PropNameMap;
		class __declspec(dllexport) PropSet
		{
			PropNameMap namedValueMap;
			std::list<Property*> namedValueList;
		public:
			void add(Property* p);
			Property& operator[](const std::string& name);
			const Property& operator[](const std::string& name) const;
			template<class T> void set(const std::string& name, T value) {
				std::stringstream ss;
				ss << value;
				namedValueMap.at(name)->setString(ss.str());
			}
			template<class T> void set(const std::string& name, const std::string& value) {
				Property* p = namedValueMap.at(name)->setString(value);
			}
		};

		//primitive property template
		template<class ValueType> class __declspec(dllexport) TParam : public Property
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
			ImageProcessor* class_name::createFilter() { return new class_name; } \
			void class_name::entryFilter() { ImageProcessor::entryFilter(#class_name, &class_name::createFilter);} \
			InitialCallback<class_name, class_name::entryFilter> class_name ## _entry_static_
		template<class T, void func()> class InitialCallback { public: InitialCallback() { func(); } };
		
		//イメージプロセッサ
		class __declspec(dllexport) ImageProcessor {
			PropSet parameters;
			std::hash_map<std::string, const cv::Mat*> inputMat;
			cv::Mat outputMat;
		protected:
			void defInputMat(std::string name);
			void defParam(Property& param);

			void setOutputMat(const cv::Mat& mat);
			cv::Mat& refOutputMat() { return outputMat; }
		public:
			ImageProcessor();
			virtual ~ImageProcessor();
			virtual void reset();
			template<class T> ImageProcessor& property(const std::string& parameter_name, T value) {
				parameters.set(parameter_name, value);
				return *this;
			}
			Property& ImageProcessor::property(const std::string& parameter_name) {
				return parameters[parameter_name];
			}
			const Property& ImageProcessor::property(const std::string& parameter_name) const {
				return parameters[parameter_name];
			}
			
			void setInputMat(const cv::Mat& mat);
			void setInputMat(std::string name, const cv::Mat& mat);
			const cv::Mat& getInputMat(std::string name = "") const;
			const cv::Mat& getOutputMat() const { return outputMat; }

			virtual void execute() = 0;
		private:
			struct Factory {
				std::string name;
				FILTER_FACTORY creator;
				Factory() : name(""),creator(0) {}
				static size_t tableSize;
				static Factory* table;
			};
		public:
			static ImageProcessor* createFilter(std::string name);
			static void entryFilter(std::string name, FILTER_FACTORY creator);
			ImageProcessor& operator << (const ImageProcessor& dst);
			const ImageProcessor& operator >> (ImageProcessor& dst) const;
			struct FilterInput {
				ImageProcessor* filter;
				std::string name;
			};
			FilterInput input(std::string name);
			const ImageProcessor& operator >> (FilterInput& dst) const;
		};

		class __declspec(dllexport) ImagePoint : public ImageProcessor {
		public:
			DECLARE_CVFILTER;
			ImagePoint(){}
			~ImagePoint(){}
			void execute();
		};
		class __declspec(dllexport) ImgProcSet : public ImageProcessor
		{
		public:
			DECLARE_CVFILTER;
		private:
			std::list<ImageProcessor*> procs;
			std::vector<ImageProcessor*> dyn_filters;
			std::hash_map<std::string, ImageProcessor*> name_to_filter;
		public:
			ImgProcSet();
			~ImgProcSet();
			ImageProcessor& add(std::string filter_class_name, std::string filter_instance_name, bool autoBind = true);
			ImageProcessor& add(ImageProcessor& proc, bool autoBind = true);
			void setParam(std::string filter_instance_name, std::string parameter_name, int value);
			ImageProcessor& operator[](std::string filter_instance_name);
			void execute();
			bool loadXml(const std::string& filename);
		};

		class __declspec(dllexport) VideoCapture : public ImageProcessor {
		public:
			DECLARE_CVFILTER;
			TParam<int> deviceIndex;
			TParam<std::string> filename;
			TParam<double> width;
			TParam<double> height;
			TParam<int> startFrame;
			TParam<int> stopFrame;
		private:
			cv::VideoCapture videoCapture;
			int frameNumber;
		public:
			VideoCapture();
			~VideoCapture();
			bool open(int deviceIndex);
			bool open(const std::string& filename);
			bool capture();
			void execute();
		};

		class __declspec(dllexport) ImageWindow : public ImageProcessor {
		public:
			DECLARE_CVFILTER;
		private:
			std::string windowName;
			int frameNumber;
			int tick;
		public:
			TParam<bool> showFrameNumber;
			TParam<bool> showFPS;
		public:
			ImageWindow();
			ImageWindow(std::string windowName);
			void execute();
		};

		class __declspec(dllexport) ColorConverter : public ImageProcessor {
		public:
			DECLARE_CVFILTER;
		private:
			TParam<int> cvtCode;
		public:
			ColorConverter();
			ColorConverter(int cvtCode);
			void execute();
		};
		class __declspec(dllexport) Resizer : public ImageProcessor {
		public:
			DECLARE_CVFILTER;
			TParam<int> width;
			TParam<int> height;
		public:
			Resizer();
			Resizer(int width, int height);
			void execute();
		};
		class __declspec(dllexport) Flipper : public ImageProcessor {
		public:
			DECLARE_CVFILTER;
		private:
			TParam<int> flipDir;
		public:
			Flipper();
			Flipper(int flipDir);
			void execute();
		};
		class __declspec(dllexport) GaussianBlur : public ImageProcessor {
		public:
			DECLARE_CVFILTER;
		private:
			TParam<int> kernelWidth;
			TParam<int> kernelHeight;
			TParam<double> sigmaX;
			TParam<double> sigmaY;
			TParam<int> borderType;
		public:
			GaussianBlur();
			GaussianBlur(
				int kernelWidth, int kernelHeight,
				double sigmaX, double sigmaY = 0.0, int borderType = cv::BORDER_DEFAULT);
			void execute();
		};
	}
}
#pragma warning(default:4251)
