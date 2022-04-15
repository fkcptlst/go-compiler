add_executable(SymbolManager_test SymbolManager.cpp)
target_link_libraries(SymbolManager_test src)
add_test(NAME SymbolManager_test COMMAND SymbolManager_test WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
