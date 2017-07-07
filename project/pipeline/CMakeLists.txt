file(GLOB_RECURSE source_list "src/*.cc")
add_library(glamorous_pipeline STATIC ${source_list})
target_link_libraries(glamorous_pipeline ${OpenCV_LIBS})
