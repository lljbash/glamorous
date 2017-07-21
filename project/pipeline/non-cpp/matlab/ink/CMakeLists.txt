add_library(glamorous_matlab_ink STATIC libinkdisp.cpp)
target_link_libraries(glamorous_matlab_ink ${OpenCV_LIBS} inkdisp mwmclmcrrt)

