find_package (glog REQUIRED)

# 源文件
file(GLOB_RECURSE src_lib_common *.cpp)
add_library(common
    ${src_lib_common}
)

# 头文件
target_include_directories(common PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries (common glog::glog)
