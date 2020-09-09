
add_library(monocular_calibration
	../include/MonocularCalibration.h MonocularCalibration.cpp
)


add_executable(calib main.cpp)
#add_executable(frame frame_capture.cpp)


target_link_libraries(calib monocular_calibration ${OpenCV_LIBS})
#target_link_libraries(frame ${OpenCV_LIBS})
