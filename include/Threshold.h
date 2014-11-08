#pragma once
#include "ImageProcessor.h"
namespace cvImagePipeline {
	namespace Filter {
		class SHARED Threshold : public ImageProcessor
		{
		public:
			enum Type {
				ThreshBinary = CV_THRESH_BINARY,
				ThreshBinaryInv = CV_THRESH_BINARY_INV,
				ThreshTrunc = CV_THRESH_TRUNC,
				ThreshToZero = CV_THRESH_TOZERO,
				ThreshToZeroInv = CV_THRESH_TOZERO_INV,
			};
		public:
			DECLARE_CVFILTER;
			TParam<double> thresh;
			TParam<double> maxval;
			TParam<Type> type;
			TParam<bool> otsu;
		public:
			Threshold();
			virtual ~Threshold();
			void execute();
		protected:
			virtual void onPropertyChange(Property& property);
		private:
			void updateThreashType();
			int thresh_type;
		};
	}
}

std::stringstream& operator << (std::stringstream& ss, cvImagePipeline::Filter::Threshold::Type& threshType)
{
	using namespace cvImagePipeline::Filter;
	switch (threshType) {
	case Threshold::ThreshBinary:
		ss << "CV_THRESH_BINARY";
		break;
	case Threshold::ThreshBinaryInv:
		ss << "CV_THRESH_BINARY_INV";
		break;
	case Threshold::ThreshTrunc:
		ss << "CV_THRESH_TRUNC";
		break;
	case Threshold::ThreshToZero:
		ss << "CV_THRESH_TOZERO";
		break;
	case Threshold::ThreshToZeroInv:
		ss << "CV_THRESH_TOZERO_INV";
		break;
	}
	return ss;
}
std::istringstream& operator >> (std::istringstream& ss, cvImagePipeline::Filter::Threshold::Type& threshType)
{
	using namespace cvImagePipeline::Filter;
	std::string s;
	ss >> s;
	if (s == "CV_THRESH_BINARY" || s == "0" || s == "BINARY" || s == "BIN") {
		threshType = Threshold::ThreshBinary;
	}
	else if (s == "CV_THRESH_BINARY_INV" || s == "1" || s == "BINARY_INV" || s == "BIN_INV" || s == "~BIN") {
		threshType = Threshold::ThreshBinaryInv;
	}
	else if (s == "CV_THRESH_TRUNC" || s == "2" || s == "TRUNC") {
		threshType = Threshold::ThreshTrunc;
	}
	else if (s == "CV_THRESH_TOZERO" || s == "3" || s == "TOZERO" || s == "ZERO") {
		threshType = Threshold::ThreshToZero;
	}
	else if (s == "CV_THRESH_TOZERO_INV" || s == "4" || s == "TOZERO_INV" || s == "ZERO_INV" || s == "~ZERO") {
		threshType = Threshold::ThreshToZeroInv;
	}
	else {
		std::cerr << s << " is undefined value for Threshold.type." << std::endl;
		exit(0);
	}
	return ss;
}
