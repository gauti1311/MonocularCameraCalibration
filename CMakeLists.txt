cmake_minimum_required(VERSION 2.6)
project(CameraCalibration)

set( CMAKE_BUILD_TYPE "Release" )
set( CMAKE_CXX_FLAGS "-std=c++11 -O3" )


include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

find_package( OpenCV 3.2.0 REQUIRED )
include_directories( 
    ${OpenCV_INCLUDE_DIRS}
)


add_subdirectory(src)
