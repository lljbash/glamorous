add_executable(testapp test.cpp app.cc)
target_link_libraries(testapp glamorous_pipeline ${PYTHON_LIBRARIES} inkdisp mwmclmcrrt luajit)
add_dependencies(testapp glamorous_pipeline)

add_library(app SHARED app.cc app_wrap.cxx)
target_link_libraries(app glamorous_pipeline ${PYTHON_LIBRARIES} inkdisp mwmclmcrrt luajit)
add_dependencies(app glamorous_pipeline)

