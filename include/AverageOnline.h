#pragma once
#include "ImageProcessor.h"
namespace cvImagePipeline {
	namespace Filter {
		//
		// �I�����C���A���S���Y���ŕ��ϒl�̒����v�Z
		// http://qiita.com/awakia/items/f58e90b3b2562d787aa5
		//
		class __declspec(dllexport) AverageOnline : public ImageProcessor
		{
		public:
			DECLARE_CVFILTER;
			TParam<int> averageCount;
		public:
			AverageOnline();
			virtual ~AverageOnline();
			void execute();
		private:
			int buffer_count;
		};
	}
}
