add_subdirectory(Common)

add_executable(SymbolManager_test SymbolManager.cpp)
target_link_libraries(SymbolManager_test common icg tcg)
add_test(NAME SymbolManager_test COMMAND SymbolManager_test WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

add_executable(icg_test icg.cpp)
target_link_libraries(icg_test common icg tcg)
add_test(NAME icg_test COMMAND icg_test WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
