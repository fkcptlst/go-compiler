file(GLOB_RECURSE src_lib_common *.cpp)

add_library(common
    ${src_lib_common}
)

target_include_directories(common PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)
