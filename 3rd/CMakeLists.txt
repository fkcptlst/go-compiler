file(GLOB_RECURSE src_lib_antlr4 ${CMAKE_SOURCE_DIR}/3rd/antlr4-runtime/src/*.cpp)
add_library(3rd
    ${src_lib_antlr4}
)

target_include_directories(3rd PUBLIC
    ${CMAKE_SOURCE_DIR}/3rd/antlr4-runtime/src
)
