cmake_minimum_required(VERSION 2.8)
set(SRC_DIR ../src/cvImagePipeline)
set(DLL_SRCS 
	${SRC_DIR}/AverageMat.cpp
	${SRC_DIR}/AverageOnline.cpp
	${SRC_DIR}/ColorConverter.cpp
	${SRC_DIR}/EqualizeHist.cpp
	${SRC_DIR}/FeatureMatcher.cpp
	${SRC_DIR}/FeatureTracker.cpp
	${SRC_DIR}/Flipper.cpp
	${SRC_DIR}/FlowTracker.cpp
	${SRC_DIR}/GaussianBlur.cpp
	${SRC_DIR}/ImageDataPtr.cpp
	${SRC_DIR}/ImagePoint.cpp
	${SRC_DIR}/ImageProcessor.cpp
	${SRC_DIR}/ImageWindow.cpp
	${SRC_DIR}/ImgProcSet.cpp
	${SRC_DIR}/LabeledObject.cpp
	${SRC_DIR}/Labeler.cpp
	${SRC_DIR}/OpticalFlowFarneback.cpp
	${SRC_DIR}/OpticalFlowPyrLK.cpp
	${SRC_DIR}/Resizer.cpp
	${SRC_DIR}/SubMat.cpp
	${SRC_DIR}/TrackingObject.cpp
	${SRC_DIR}/VideoCapture.cpp
	${SRC_DIR}/createVideoAverageImage.cpp
	${SRC_DIR}/cvUtils.cpp
	${SRC_DIR}/xmlLoader.cpp
	${SRC_DIR}/pugixml-1.4/src/pugixml.cpp
	${SRC_DIR}/Convert.cpp
	${SRC_DIR}/MaskCopy.cpp
	${SRC_DIR}/Threshold.cpp
	)
set(OPENCV_BUILD_DIR /opencv/build)
set(OPECV_REL_LIB
	opencv_calib3d248.lib
	opencv_contrib248.lib
	opencv_core248.lib
	opencv_features2d248.lib
	opencv_flann248.lib
	opencv_gpu248.lib
	opencv_highgui248.lib
	opencv_imgproc248.lib
	opencv_legacy248.lib
	opencv_ml248.lib
	opencv_nonfree248.lib
	opencv_objdetect248.lib
	opencv_ocl248.lib
	opencv_photo248.lib
	opencv_stitching248.lib
	opencv_superres248.lib
	opencv_ts248.lib
	opencv_video248.lib
	opencv_videostab248.lib
	opencv_nonfree248.lib
)
include_directories(
	${OPENCV_BUILD_DIR}/include
	${OPENCV_BUILD_DIR}/include/opencv
	${OPENCV_BUILD_DIR}/include/opencv2
	../include
	)
link_directories(
	${OPENCV_BUILD_DIR}/x86/vc10/lib
	${OPENCV_BUILD_DIR}/x86/vc10/staticlib
	)

add_library(cvImagePipeline SHARED ${DLL_SRCS})
target_link_libraries(cvImagePipeline ${OPECV_REL_LIB})

add_executable(test ../test/test.cpp)
target_link_libraries(test cvImagePipeline ${OPECV_REL_LIB})

add_executable(sample ../sample/capture.cpp)
target_link_libraries(sample cvImagePipeline ${OPECV_REL_LIB})
