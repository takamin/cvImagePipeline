#pragma once
#include <opencv2/opencv.hpp>

namespace cvImagePipeline {
	// IplImageのimageDataをカプセル化
	class ImageDataPtr {
	private:
		IplImage* image;
		void* vp;
		size_t element_size;
	public:
		//デフォルトコンストラクタ
		inline ImageDataPtr() 
			: image(0), vp(0), element_size(0)
		{
		}
		//コンストラクタ
		inline ImageDataPtr(IplImage* image) {
			setImage(image); 
		}
		//IplImageの代入
		inline ImageDataPtr& operator = (IplImage* image) { 
			setImage(image); 
			return *this;
		}
		//走査ポインタを１画素分右へ進める。
		inline const ImageDataPtr& operator ++ () { 
			inc(); 
			return *this;
		}
		inline const ImageDataPtr& operator ++ (int) { 
			inc(); 
			return *this;
		}
		//走査ポインタを上からy行目の左端へセットする。
		inline void setLine(int y) { 
			vp = image->imageData + image->widthStep * y;
		}
		//ポインタ位置の値をdoubleとして取り出す。
		inline operator double() { 
			return getValue(); 
		}
		//ポインタ位置へ値を格納する。
		inline double operator = (double value) { 
			setValue(value); 
			return value; 
		}
	private:
		inline void setImage(IplImage* image) {
			this->image = image;
			vp = image->imageData;
			switch(image->depth) {
				case IPL_DEPTH_8U:
					element_size = sizeof(unsigned char);
					break;
				case IPL_DEPTH_16U:
					element_size = sizeof(unsigned short);
					break;
				case IPL_DEPTH_32F:
					element_size = sizeof(float);
					break;
				default: break;
			}
		}
		inline void inc() { add(1); }
		inline void add(int n) {
			vp = (char*)vp + n * element_size;
		}
		inline double getValue() {
			double fvalue = 0;
			switch(image->depth) {
				case IPL_DEPTH_8U:
					fvalue = (double)*(unsigned char*)vp;
					break;
				case IPL_DEPTH_16U:
					fvalue = (double)*(unsigned short*)vp;
					break;
				case IPL_DEPTH_32F:
					fvalue = (double)*(float*)vp;
					break;
				default:
					break;
			}
			return fvalue;
		}
		inline void setValue(double fvalue) {
			switch(image->depth) {
				case IPL_DEPTH_8U:  
					*(unsigned char*)vp  = (unsigned char)fvalue; 
					break;
				case IPL_DEPTH_16U: 
					*(unsigned short*)vp = (unsigned short)fvalue;
					break;
				case IPL_DEPTH_32F: 
					*(float*)vp = (float)fvalue; 
					break;
				default: break;
			}
		}
	};

	//// IplImageのimageDataをカプセル化
	//class ImageDataPtr {
	//private:
	//	IplImage* image;
	//	void* vp;
	//	size_t element_size;
	//public:
	//	inline ImageDataPtr() : image(0), vp(0), element_size(0) { }
	//	inline ImageDataPtr(IplImage* image) { setImage(image); }
	//	inline ImageDataPtr& operator = (IplImage* image) { setImage(image); return *this;}
	//	inline const ImageDataPtr& operator ++ () { inc(); return *this;}
	//	inline const ImageDataPtr& operator ++ (int) { inc(); return *this;}
	//	inline void setLine(int y) { vp = image->imageData + image->widthStep * y;}
	//	inline operator double() { return getValue(); }
	//	inline double operator = (double value) { setValue(value); return value; }
	//private:
	//	inline void setImage(IplImage* image) {
	//		this->image = image;
	//		vp = image->imageData;
	//		switch(image->depth) {
	//			case IPL_DEPTH_8U:
	//				element_size = sizeof(unsigned char);
	//				break;
	//			case IPL_DEPTH_16U:
	//				element_size = sizeof(unsigned short);
	//				break;
	//			case IPL_DEPTH_32F:
	//				element_size = sizeof(float);
	//				break;
	//			default: break;
	//		}
	//	}
	//	inline void inc() { add(1); }
	//	inline void add(int n) {
	//		vp = (char*)vp + n * element_size;
	//	}
	//	inline double getValue() {
	//		double fvalue = 0;
	//		switch(image->depth) {
	//			case IPL_DEPTH_8U:
	//				fvalue = (double)*(unsigned char*)vp;
	//				break;
	//			case IPL_DEPTH_16U:
	//				fvalue = (double)*(unsigned short*)vp;
	//				break;
	//			case IPL_DEPTH_32F:
	//				fvalue = (double)*(float*)vp;
	//				break;
	//			default:
	//				break;
	//		}
	//		return fvalue;
	//	}
	//	inline void setValue(double fvalue) {
	//		switch(image->depth) {
	//			case IPL_DEPTH_8U:  *(unsigned char*)vp  = (unsigned char)fvalue; break;
	//			case IPL_DEPTH_16U: *(unsigned short*)vp = (unsigned short)fvalue;break;
	//			case IPL_DEPTH_32F: *(float*)vp = (float)fvalue; break;
	//			default: break;
	//		}
	//	}
	//};
}
