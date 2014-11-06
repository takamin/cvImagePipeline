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

add_executable(test test/test.cpp)
target_link_libraries(test cvImagePipeline ${OpenCV_LIBS})

add_executable(sample sample/capture.cpp)
target_link_libraries(sample cvImagePipeline ${OpenCV_LIBS})
