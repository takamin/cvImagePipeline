External Reference Specification
================================

## Image Processors

### class AbsDiff

2�̉摜�̍��̐�Βl���o�͂���B
cv::absdiff �Ɠ����̏����B

#### Input

* (�K��̓���) - cv::absdiff�̑������B�������摜
* `"subImage"` - cv::absdiff�̑������B�����摜


### class Convert

�r�b�g�[�x�ƃ`���l������ϊ�����B
cv::Mat::convertTo �Ɠ����B

#### Property

* int rtype - �o�͉摜�̃r�b�g�[�x�ƃ`���l���BCV_8UC1,CV_32FC3 �Ȃǂ�CV_MAKETYPE(depth,channels)�ō쐬�ł���l�B
* double alpha - �r�b�g�[�x�ϊ����ɁA���̉�f�l�ւ�����l�B
* double beta - �r�b�g�[�x�ϊ����Aalpha����������ɑ����l�B

### Dilate

�c���Bcv::dilate�Ɠ��������B

#### Property

* int iterations - �J��Ԃ��񐔁B�K��l��1

### Erode

�k���Bcv::erode�Ɠ��������B

#### Property

* int iterations - �J��Ԃ��񐔁B�K��l��1

### EqualizeHist

�q�X�g�O�����̕�����

### Threshold

��l���Bcv::threshold �Ɠ����B

### VideoCapture

�摜�̓��́B���s����邽�тɁA�J�����A����t�@�C������V�����t���[�������o���ďo�͂���B


### ImageWindow

���O�t���E�B���h�E�ւ̉摜�\���B

#### Property

* std::string windowName - �E�B���h�E���B

