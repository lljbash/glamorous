add_executable(testapp test.cpp)
target_link_libraries(testapp glamorous_pipeline mwmclmcrrt inkdisp)
add_dependencies(testapp glamorous_pipeline)
