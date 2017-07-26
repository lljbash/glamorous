add_executable(testapp test.cpp app.cc)
target_link_libraries(testapp glamorous_pipeline ${PYTHON_LIBRARIES} inkdisp mwmclmcrrt luajit)
add_dependencies(testapp glamorous_pipeline)

add_executable(server server.cpp app.cc)
target_link_libraries(server glamorous_pipeline ${PYTHON_LIBRARIES} inkdisp mwmclmcrrt luajit)
add_dependencies(server glamorous_pipeline)

add_library(app SHARED app.cc app_wrap.cxx)
target_link_libraries(app glamorous_pipeline ${PYTHON_LIBRARIES} inkdisp mwmclmcrrt luajit)
add_dependencies(app glamorous_pipeline)

