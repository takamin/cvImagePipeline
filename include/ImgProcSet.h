#pragma once
#include <hash_map>
#include <list>
#include <vector>
#include <string>
#include "ImageProcessor.h"
#include "../src/cvImagePipeline/pugixml-1.4/src/pugixml.hpp"
#pragma warning(disable:4251)
namespace cvImagePipeline {
	namespace Filter {
		class __declspec(dllexport) ImgProcSet : public ImageProcessor
		{
		public:
			DECLARE_CVFILTER;
		private:
			std::list<ImageProcessor*> procs;
			std::vector<ImageProcessor*> dyn_filters;
			std::hash_map<std::string, ImageProcessor*> name_to_filter;
			const ImageProcessor* lastAutoProcessor;
		public:
			ImgProcSet();
			~ImgProcSet();
			ImageProcessor& add(std::string filter_class_name, std::string filter_instance_name, bool autoBind = true);
			ImageProcessor& add(ImageProcessor& proc, bool autoBind = true);
			void setParam(std::string filter_instance_name, std::string parameter_name, int value);
			ImageProcessor& operator[](const std::string& filter_instance_name);
			const ImageProcessor& operator[](const std::string& filter_instance_name) const;
			ImageProcessor& processor(const std::string& filter_instance_name);
			const ImageProcessor& processor(const std::string& filter_instance_name) const;
			void execute();
			bool loadXml(const std::string& filename);
			virtual void putMarkdown(std::ostream& stream);
		protected:
			//最後に追加されたプロセッサへの参照を返す
			ImageProcessor& getLastProc();
		private:
			void addProcessorByXmlNode(pugi::xml_node processor);
			void setInputMatByXmlNode(ImageProcessor& imageProcessor, pugi::xml_node input);
		};
		inline const ImageProcessor& ImgProcSet::operator[](const std::string& filter_instance_name) const {
			return (*(ImgProcSet*)this)[filter_instance_name];
		}
		inline ImageProcessor& ImgProcSet::processor(const std::string& filter_instance_name) {
			return (*this)[filter_instance_name];
		}
		inline const ImageProcessor& ImgProcSet::processor(const std::string& filter_instance_name) const {
			return (*(ImgProcSet*)this)[filter_instance_name];
		}
	}
}