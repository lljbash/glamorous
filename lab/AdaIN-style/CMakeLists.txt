cmake_minimum_required(VERSION 2.8)
project( main )

IF (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -pagezero_size 10000 -image_base 100000000")
ELSE()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall")
ENDIF()

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} ~/torch/install/include )
link_directories( ~/torch/install/lib )
add_executable( main main.cpp style_trans.cc )
target_link_libraries( main ${OpenCV_LIBS} luajit )
