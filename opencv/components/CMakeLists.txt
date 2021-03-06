cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(components)
include(DirectShowCam/BaseClasses/FindDirectShow.cmake)
include(PGRCamera/cmake/FindFlyCapture.cmake)

add_subdirectory(TkCalibGUI)
add_subdirectory(Affine)
add_subdirectory(BackGroundSubtractionSimple)
add_subdirectory(Binarization)
add_subdirectory(CameraViewer)
add_subdirectory(Chromakey)
add_subdirectory(Dilationerosion)



if (WIN32)
  if (${CMAKE_GENERATOR} MATCHES [[Visual Studio (9|10) (2008|2010)(| Win64| ARM)]])
    if(DIRECTSHOW_FOUND)
      add_subdirectory(DirectShowCam)
    endif()
  else()
    add_subdirectory(MFCamera)
  endif()
endif()

add_subdirectory(Edge)
add_subdirectory(Findcontour)
add_subdirectory(Flip)
add_subdirectory(Histogram)
add_subdirectory(Houghline)
add_subdirectory(ImageCalibration)
add_subdirectory(ImageSubstraction)
add_subdirectory(ObjectTracking)
add_subdirectory(OpenCVCamera)
if(FLYCAPTURE_FOUND)
  add_subdirectory(PGRCamera)
endif()
add_subdirectory(Perspective)
add_subdirectory(RockPaperScissors)
add_subdirectory(Rotate)
add_subdirectory(Scale)
add_subdirectory(Sepia)
add_subdirectory(SubStractCaptureImage)
add_subdirectory(Template)
add_subdirectory(Translate)
