# cvImagePipeline - OpenCV�摜�����t�B���^�V�[�P���X���C�u�����B

## �T�v

[cvImagePipeline](https://github.com/takamin/cvImagePipeline)
�́AOpenCV�̉摜�������[�`����g�ݍ��킹�āA��A�̉摜�������\���ł��郉�C�u�����ł��B
���̃��C�u�����ł́AOpenCV�Ɋ܂܂��摜�������[�`�����A���̓��o�̓C���[�W(`cv::Mat`)��p�����[�^�ƂƂ��ɁA
�u[�摜�����v���Z�b�T](#ImageProcessor)�v�ƌĂԃN���X�Ɏ������܂��B

__���o�͂̎��R�Ȑڑ�__
�摜�����v���Z�b�T�̏o�͂Ɠ��͂�ڑ����āA��A�̉摜�������\�����܂��B
���̐ڑ��́A���s���ɂ��ύX�\�ł��B

__�g���\__  
�������̔ėp�I�ȁi�P���ȁj�摜�����v���Z�b�T��[�����ς�](#processor)�ł����A����Ȃ����̂̓��[�U�[�Ǝ��ɍ쐬�\�ł��B

__�K�w���E���W���[����__  
�ڑ����ꂽ��A�̉摜�������܂��A�摜�����v���Z�b�T�ł��邽�߁A�摜�����̊K�w���A���W���[�������ȒP�ɉ\�ł��B

__XML�t�@�C������̓���__  
�v���Z�b�T�̒P���Ȑڑ���[XML�t�@�C����](https://github.com/takamin/cvImageBlock/blob/master/sample/sample.xml)�ɋL�q�ł��܂��B
XML�̓ǂݍ��݊ւ��Ă�[�T���v���v���O����](https://github.com/takamin/cvImageBlock/blob/master/sample/capture.cpp)���Q�Ƃ��Ă��������B

### �J����

* �o�̓^�[�Q�b�g Win32 DLL
* �����J���� VisualStudio Express 2013 for Windows Desktop
* �v���b�g�t�H�[�� Win32(x86)
* OpenCV 2.4.8�ƃ����N���܂��B[DOWNLOADS|OpenCV](http://opencv.org/downloads.html)����_�E�����[�h����`C:\opencv`�ɓW�J�i`C:\opencv\build`�f�B���N�g���������ԁj�B
* ���s���ɂ� c:\opencv\build\x86\vc12\bin ��PATH��ʂ��K�v������܂��B
* xml�p�[�T�[�Ƃ��āA[pugixml-1.4](http://pugixml.org/)�𗘗p���Ă��܂��B
* [capture.cpp](https://github.com/takamin/cvImagePipeline/blob/master/sample/capture.cpp).


### �r���h���@

�r���h�� [src/cvImagePipeline](https://github.com/takamin/cvImageBlock/blob/master/src)�̃\�����[�V�����ōs���܂��B

CMake�p�ɁACMakeLists.txt���L�q���Ă��܂����A���퓮��s���ł��B

## �ڍא���

### <a name="ImageProcessor"></a>�摜�����v���Z�b�T

�摜�����v���Z�b�T��[���ۃN���X`ImageProcessor`]((https://github.com/takamin/cvImageBlock/blob/master/include/ImageProcessor.h))���p�����ċL�q����܂��B

* �ЂƂ̏o�͉摜(`const cv::Mat&`)��񋟂��܂��B
* �����̓��͉摜(`const cv::Mat&`)���Q�Ƃ��܂��B
* �����̃v���p�e�B��ێ��ł��܂��B�����̃p�����[�^�Ƃ��Ďg�p�ł��܂��B
* �o�͉摜�͕����̃v���Z�b�T����Q�Ɖ\�ł��B
* ���͉摜�ƃv���p�e�B�ɂ͖��O�ɂ��A�N�Z�X���\�B

#### �N���X���ɂ����s�����I�C���X�^���X����

�N���X�錾��DECLARE_CVFILTER�A��`����IMPLEMENT_CVFILTER �}�N�����g�p����΁A�N���X�̖��O�ŃC���X�^���X�𐶐��ł��܂��B
���I�����ɑΉ����Ă��Ȃ��ꍇ�́A��q��XML�t�@�C������̍\�z�͂ł��܂���B


### <a name="processors"></a>�����ς݊�{�v���Z�b�T

�ȉ��̃v���Z�b�T����������Ă��܂��B

|�N���X				|�T�v															|
|:---				|:--															|
| VideoCapture		| �J�����܂��͓���t�@�C���̃L���v�`��(==`cv::VideoCapture`)	|
| ImageWindow		| ��ʕ\��(== `imshow`)	|
| Convert			| �t�H�[�}�b�g�ϊ�(== `cv::Mat::convertTo`)	| 
| ColorConverter	| �`�����l�����ϊ�(== `cv::cvtColor`)	|
| EqualizeHist		| �q�X�g�O�������R��(== `cv::equalizeHist`)	|
| GaussianBlur		| �K�E�V�A��������(== `cv::GaussianBlur`)	|
| Flipper			| ���]�B(== `cv::flip`)	|
| Resizer			| ���T�C�Y(== `cv::resize`)	|
| BackgroundSubtractor	| MOG/MOG2/GMG �w�i����	|
| RunningAvg		| ��������(== `cv::runningAvg`)	|
| AbsDiff			| ��Βl����(== `cv::absdiff`)	|
| SubMat			| ����(== `cv::sub`)	|
| Threshold			| 2�l��(== `cv::threshold`)	|
| Dilate			| �摜�̖c��(== `cv::dilate`)	|
| Erode				| �摜�̏k��(== `cv::erode`)	|
| MaskCopy			| �}�X�N����(== `cv::copyTo`)	|
| ImagePoint		| ������(== `cv::copyTo`)�B���͉摜�����̂܂܏o�͂��܂��B�摜�̎��o���|�C���g�Ƃ��ė��p�\�B	|
| OpticalFlowFarneback	| Farneback�̖��ȃI�v�e�B�J���t���[���v�Z(== `cv::calcOpticalFlowFarneback`)���܂��B�t���[���������邽�߂̃v���Z�b�T�������N���X�ɗp�ӂ��Ă��܂��B	|
| OpticalFlowPyrLK	| �a�ȃI�v�e�B�J���t���[(== `cv::calcOpticalFlowPyrLK`)���v�Z���A�����_��`�悷��T���v���ł��B|
| FitInGrid			| �����̉摜���p�l����ɕ��ׂ�1���ɂ܂Ƃ߂�v���Z�b�T�B	|
| ImgProcSet		| �C�ӂ̉摜�����v���Z�b�T�ō\���\�Ȕėp�̉摜�����v���Z�b�T�B	|

### �摜�����v���Z�b�T�Z�b�g

�C�ӂ̕����̉摜�����v���Z�b�T��ێ����A�����������s���v���Z�b�T�ł��B
��[`ImgProcSet`�N���X]((https://github.com/takamin/cvImageBlock/blob/master/include/ImgProcSet.h))�ł��B

�����ɕێ�����v���Z�b�T�Ԃ̐ڑ���؂�ւ��\�B

* �����̉摜�����v���Z�b�T��ێ����܂��B
* XML�t�@�C����ǂݍ���ŁA�\���ł��܂��B
* �����̃v���Z�b�T�̐ڑ����ǂ̂悤�ɂȂ��Ă��Ă��A�摜�����͒ǉ����ꂽ�����Ŏ��s����܂��B
