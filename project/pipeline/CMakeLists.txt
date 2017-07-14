file(GLOB_RECURSE source_list "src/*.cc")
include_directories(${CMAKE_SOURCE_DIR}/pipeline/src)
add_library(glamorous_pipeline STATIC ${source_list})
target_link_libraries(glamorous_pipeline ${OpenCV_LIBS})
