cmake_minimum_required(VERSION 2.8)
project(glamorous)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -pthread -Wno-misleading-indentation")

set(OpenCV_STATIC ON)
find_package(OpenCV REQUIRED)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

link_directories(/usr/local/MATLAB/R2017a/runtime/glnxa64)
link_directories(/usr/local/MATLAB/R2017a/bin/glnxa64)
link_directories(${CMAKE_SOURCE_DIR}/pipeline/non-cpp/matlab/ink)

include_directories(/usr/local/MATLAB/R2017a/extern/include)
include_directories(${CMAKE_SOURCE_DIR}/pipeline/include)

add_subdirectory(pipeline)
add_subdirectory(app)
