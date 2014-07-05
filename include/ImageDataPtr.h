#pragma once
#include <opencv2/opencv.hpp>

namespace cvImagePipeline {
	// IplImage��imageData���J�v�Z����
	class ImageDataPtr {
	private:
		IplImage* image;
		void* vp;
		size_t element_size;
	public:
		//�f�t�H���g�R���X�g���N�^
		inline ImageDataPtr() 
			: image(0), vp(0), element_size(0)
		{
		}
		//�R���X�g���N�^
		inline ImageDataPtr(IplImage* image) {
			setImage(image); 
		}
		//IplImage�̑��
		inline ImageDataPtr& operator = (IplImage* image) { 
			setImage(image); 
			return *this;
		}
		//�����|�C���^���P��f���E�֐i�߂�B
		inline const ImageDataPtr& operator ++ () { 
			inc(); 
			return *this;
		}
		inline const ImageDataPtr& operator ++ (int) { 
			inc(); 
			return *this;
		}
		//�����|�C���^���ォ��y�s�ڂ̍��[�փZ�b�g����B
		inline void setLine(int y) { 
			vp = image->imageData + image->widthStep * y;
		}
		//�|�C���^�ʒu�̒l��double�Ƃ��Ď��o���B
		inline operator double() { 
			return getValue(); 
		}
		//�|�C���^�ʒu�֒l���i�[����B
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

	//// IplImage��imageData���J�v�Z����
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
