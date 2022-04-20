add_executable(symbol_test symbol.cpp)
target_link_libraries(symbol_test common icg tcg)
add_test(NAME symbol_test COMMAND symbol_test WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
