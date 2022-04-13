add_executable(example_test example.cpp)
target_link_libraries(example_test src)
add_test(NAME example_test COMMAND example_test WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/build)