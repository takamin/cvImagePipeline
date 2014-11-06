cmake_minimum_required(VERSION 2.8)
project( cvImagePipeline )
set(DLL_SRCS 
    src/cvImagePipeline/AbsDiff.cpp
    src/cvImagePipeline/BackgroundSubtractor.cpp
    src/cvImagePipeline/ColorConverter.cpp
    src/cvImagePipeline/Convert.cpp
    src/cvImagePipeline/DepthTo32F.cpp
    src/cvImagePipeline/Dilate.cpp
    src/cvImagePipeline/EqualizeHist.cpp
    src/cvImagePipeline/Erode.cpp
    src/cvImagePipeline/FeatureMatcher.cpp
    src/cvImagePipeline/FeatureTracker.cpp
    src/cvImagePipeline/FitInGrid.cpp
    src/cvImagePipeline/Flipper.cpp
    src/cvImagePipeline/FlowTracker.cpp
    src/cvImagePipeline/GaussianBlur.cpp
    src/cvImagePipeline/ImageDataPtr.cpp
    src/cvImagePipeline/ImagePoint.cpp
    src/cvImagePipeline/ImageProcessor.cpp
    src/cvImagePipeline/ImageWindow.cpp
    src/cvImagePipeline/ImgProcSet.cpp
    src/cvImagePipeline/ImgProcThread.cpp
    src/cvImagePipeline/LabeledObject.cpp
    src/cvImagePipeline/Labeler.cpp
    src/cvImagePipeline/MaskCopy.cpp
    src/cvImagePipeline/OpticalFlowFarneback.cpp
    src/cvImagePipeline/OpticalFlowPyrLK.cpp
    src/cvImagePipeline/RegionOfInterest.cpp
    src/cvImagePipeline/Resizer.cpp
    src/cvImagePipeline/RunningAvg.cpp
    src/cvImagePipeline/SubMat.cpp
    src/cvImagePipeline/SuperImpose.cpp
    src/cvImagePipeline/Threshold.cpp
    src/cvImagePipeline/TrackingObject.cpp
    src/cvImagePipeline/VideoCapture.cpp
    src/cvImagePipeline/createVideoAverageImage.cpp
    src/cvImagePipeline/cvUtils.cpp
    src/cvImagePipeline/xmlLoader.cpp
    src/cvImagePipeline/pugixml-1.4/src/pugixml.cpp
    )

find_package( OpenCV REQUIRED )

include_directories( include )
add_library(cvImagePipeline SHARED ${DLL_SRCS})
target_link_libraries(cvImagePipeline ${OpenCV_LIBS})

add_executable(test test/test.cpp)
target_link_libraries(test cvImagePipeline ${OpenCV_LIBS})

add_executable(sample sample/capture.cpp)
target_link_libraries(sample cvImagePipeline ${OpenCV_LIBS})
