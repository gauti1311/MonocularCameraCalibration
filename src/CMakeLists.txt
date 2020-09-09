
add_library(monocular_calibration
	MonocularCalibration.h MonocularCalibration.cpp
)

add_executable(calibration main.cpp)
add_executable(framecapture frame_capture.cpp)


target_link_libraries(calibration monocular_calibration ${OpenCV_LIBS})
target_link_libraries(frame ${OpenCV_LIBS})
