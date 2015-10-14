cmake_minimum_required(VERSION 2.8)
project( cvImagePipeline )
set(SOURCES 
    source/AbsDiff.cpp
    source/BackgroundSubtractor.cpp
    source/ColorConverter.cpp
    source/Convert.cpp
    source/DepthTo32F.cpp
    source/Dilate.cpp
    source/EqualizeHist.cpp
    source/Erode.cpp
    source/FeatureMatcher.cpp
    source/FeatureTracker.cpp
    source/FitInGrid.cpp
    source/Flipper.cpp
    source/FlowTracker.cpp
    source/GaussianBlur.cpp
    source/ImageDataPtr.cpp
    source/ImagePoint.cpp
    source/ImageProcessor.cpp
    source/ImageWindow.cpp
    source/ImgProcSet.cpp
    source/ImgProcThread.cpp
    source/LabeledObject.cpp
    source/Labeler.cpp
    source/MaskCopy.cpp
    source/OpticalFlowFarneback.cpp
    source/OpticalFlowPyrLK.cpp
    source/RegionOfInterest.cpp
    source/Resizer.cpp
    source/RunningAvg.cpp
    source/SubMat.cpp
    source/SuperImpose.cpp
    source/Threshold.cpp
    source/TrackingObject.cpp
    source/VideoCapture.cpp
    source/createVideoAverageImage.cpp
    source/cvUtils.cpp
    source/xmlLoader.cpp
    source/pugixml-1.4/src/pugixml.cpp
    )

find_package( OpenCV REQUIRED )

include_directories( include )
add_library(cvImagePipeline SHARED ${SOURCES})
target_link_libraries(cvImagePipeline ${OpenCV_LIBS})
add_definitions(-DBUILD_DLL)

add_executable(test test/test.cpp)
target_link_libraries(test cvImagePipeline ${OpenCV_LIBS})

add_executable(sample sample/capture.cpp)
target_link_libraries(sample cvImagePipeline ${OpenCV_LIBS})

install (TARGETS cvImagePipeline DESTINATION .)
install (FILES
    include/AbsDiff.h
    include/BackgroundSubtractor.h
    include/ColorConverter.h
    include/Convert.h
    include/DepthTo32F.h
    include/Dilate.h
    include/EqualizeHist.h
    include/Erode.h
    include/FeatureMatcher.h
    include/FeatureTracker.h
    include/FitInGrid.h
    include/Flipper.h
    include/FlowTracker.h
    include/GaussianBlur.h
    include/ImageDataPtr.h
    include/ImagePoint.h
    include/ImageProcessor.h
    include/ImageWindow.h
    include/ImgProcSet.h
    include/ImgProcThread.h
    include/LabeledObject.h
    include/Labeler.h
    include/Labeling.h
    include/MaskCopy.h
    include/OpticalFlowFarneback.h
    include/OpticalFlowPyrLK.h
    include/RegionOfInterest.h
    include/Resizer.h
    include/RunningAvg.h
    include/SubMat.h
    include/SuperImpose.h
    include/Threshold.h
    include/TrackingObject.h
    include/VideoCapture.h
    include/cvImagePipeline.h
    include/cvUtils.h
    include/pugiconfig.hpp
    include/pugixml.hpp
    DESTINATION include
)
