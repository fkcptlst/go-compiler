add_subdirectory(Common)
# add_subdirectory(TCG)

add_executable(icg_test icg.cpp)
target_link_libraries(icg_test common icg tcg)
add_test(NAME icg_test COMMAND icg_test WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
