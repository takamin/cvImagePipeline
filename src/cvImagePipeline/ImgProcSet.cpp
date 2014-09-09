#include "ImgProcSet.h"
#include <typeinfo>
#include <time.h>
using namespace std;

namespace cvImagePipeline {
	namespace Filter {
		///////////////////////////////////////////////////////////////////////////
		// class ImgProcSet

		IMPLEMENT_CVFILTER(ImgProcSet);

		ImgProcSet::ImgProcSet()
			: lastAutoProcessor(0)
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
				filter->setName(getName() + "." + filter_instance_name);
				add(*filter, autoBind);
			} else {
				std::cerr << "ImageProcessor \"" << filter_class_name << "\" is not exist in "
					<< getName() << " instance of class " << std::string(typeid(*this).name()) << "."
					<< std::endl;
				throw new std::exception("ProcessorNotFound");
			}
			return *filter;
		}
		ImageProcessor& ImgProcSet::add(ImageProcessor& proc, bool autoBind)
		{
			if(autoBind) {
				std::list<ImageProcessor*>::reverse_iterator back = procs.rbegin();
				if(back != procs.rend()) {
					ImageProcessor* last = *back;
					*last >> proc;
				} else {
					proc.setInputMat(getInputMat());
				}
				refOutputMat() = proc.getOutputMat();
				lastAutoProcessor = &proc;
			}
			procs.push_back(&proc);
			return proc;
		}
		void ImgProcSet::setParam(
			std::string filter_instance_name,
			std::string parameter_name, int value)
		{
			(*this)[filter_instance_name].property(parameter_name, value);
		}
		ImageProcessor& ImgProcSet::operator[](
			std::string filter_instance_name)
		{
			ImageProcessor* filter = 0;
			if(name_to_filter.count(filter_instance_name) > 0) {
				filter = name_to_filter[filter_instance_name];
			} else {
				std::cerr << "ImageProcessor named \"" << filter_instance_name << "\" is not exist in "
					<< getName() << " instance of class " << std::string(typeid(*this).name()) << "."
					<< std::endl;
				throw new std::exception("ProcessorInstanceNotFound");
			}
			return *filter;
		}
		
		//最後に追加されたプロセッサへの参照を返す
		ImageProcessor& ImgProcSet::getLastProc() {
			std::list<ImageProcessor*>::reverse_iterator last = procs.rbegin();
			return *(*last);
		}
		
		void ImgProcSet::execute()
		{
			if (isEnable()) {
				std::list<ImageProcessor*>::iterator proc = procs.begin();
				while (proc != procs.end()) {
					if ((*proc)->isEnable()) {
						try {
							(*proc)->execute();
						}
						catch (...) {
							cerr
								<< "exception caught in " << getName()
								<< "from " << (*proc)->getName() << "."
								<< endl;
						}
					}
					proc++;
				}
				if (lastAutoProcessor != 0) {
					setOutputMat(lastAutoProcessor->getOutputMat());
				}
			}
		}
		void ImgProcSet::putMarkdown(std::ostream& stream) {
			ImageProcessor::putMarkdown(stream);
			stream << "----" << std::endl;
			stream << std::endl;
			std::list<ImageProcessor*>::iterator proc = procs.begin();
			for(; proc != procs.end(); proc++) {
				(*proc)->putMarkdown(stream);
			}
		}

		using namespace pugi;
		/**
		 * create procesor pipeline from xml file. 
		 */
		bool ImgProcSet::loadXml(const std::string& filename) {
			xml_document document;
			xml_parse_result result = document.load_file(filename.c_str());
			if(result.status != status_ok) {
				return false;
			}
			xml_node root_node = document.document_element();
#ifdef _DEBUG
			cerr << "[ImgProcSet::loadXml]" << "root node name = " << root_node.name() << endl;
#endif
			pugi::xml_attribute root_name_attr = root_node.attribute("name");
			setName(root_name_attr.as_string());

			xpath_node_set processor_node_set = root_node.select_nodes("Processor");
			xpath_node_set::const_iterator processor = processor_node_set.begin();
			while(processor != processor_node_set.end()) {
				addProcessorByXmlNode(processor->node());
				processor++;
			}
			return true;
		}
		void ImgProcSet::addProcessorByXmlNode(pugi::xml_node processor)
		{
			// Attr processor class name
			xml_attribute attr_proc_class = processor.attribute("class");
			// Attr instance name
			xml_attribute attr_proc_name = processor.attribute("name");
#ifdef _DEBUG
			cerr << "[ImgProcSet::addProcessorByXmlNode]  " << "Processor "
				<< attr_proc_class.name() << " = " << attr_proc_class.as_string() << ", "
				<< attr_proc_name.name() << " = " << attr_proc_name.as_string() << endl;
#endif

			// Attr autoBind
			xml_attribute attr_bind = processor.attribute("autoBind");

			string autoBindAttrName = string(attr_bind.name());
			bool autoBind = true;
			if(autoBindAttrName == "") {
#ifdef _DEBUG
				cerr << "property autoBind not specified. default=true" << std::endl;
#endif
			} else {
				autoBind  = attr_bind.as_bool();
			}
#ifdef _DEBUG
			cerr << "property autoBind = \"" << autoBind << "\"" << std::endl;
#endif

			// create and add.
			ImageProcessor& proc = add(
					attr_proc_class.as_string(),
					attr_proc_name.as_string(),
					autoBind);

			//
			// Property
			//
			xpath_node_set property_node_set = processor.select_nodes("Property");
			xpath_node_set::const_iterator property = property_node_set.begin();
			while(property != property_node_set.end()) {
				proc.setPropertyByXmlNode(property->node());
				property++;
			}

			//
			// Input
			//
			xpath_node_set input_node_set = processor.select_nodes("Input");
			xpath_node_set::const_iterator input = input_node_set.begin();
			while(input != input_node_set.end()) {
				setInputMatByXmlNode(proc, input->node());
				input++;
			}

		}
		void ImgProcSet::setInputMatByXmlNode(ImageProcessor& imageProcessor, pugi::xml_node input) {
			string srcProcName = string(input.attribute("from").as_string());
			string dstInMatName = string(input.attribute("to").as_string());
#ifdef _DEBUG
			cerr << "[ImageProcessor::setInputMatByXmlNode]    " << "Input "
				<< "from \"" << srcProcName << "\", to " << dstInMatName << endl;
#endif
			(*this)[srcProcName] >> imageProcessor.input(dstInMatName);
		}

		void ImageProcessor::setPropertyByXmlNode(xml_node property) {
			xml_attribute attr_prop_name = property.attribute("name");
			xml_attribute attr_prop_value = property.attribute("value");
#ifdef _DEBUG
			cerr << "[ImageProcessor::setPropertyByXmlNode]    " << "Property "
				<< attr_prop_name.name() << " = " << attr_prop_name.as_string() << ", "
				<< attr_prop_value.name() << " = " << attr_prop_value.as_string() << endl;
#endif
			setPropertyAsString(attr_prop_name.as_string(), attr_prop_value.as_string());
		}
	}
}
