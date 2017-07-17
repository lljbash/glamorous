add_executable(testapp test.cpp app.cc)
target_link_libraries(testapp glamorous_pipeline mwmclmcrrt inkdisp)
add_dependencies(testapp glamorous_pipeline)

find_package(PythonLibs)
include_directories(${PYTHON_INCLUDE_PATH})

add_library(app SHARED app.cc app_wrap.cxx)
target_link_libraries(app ${PYTHON_LIBRARIES} glamorous_pipeline mwmclmcrrt inkdisp)
add_dependencies(app glamorous_pipeline)

