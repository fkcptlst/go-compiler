cmake_minimum_required(VERSION 3.5)
project(tjugo)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -O3 -g")

enable_testing()

add_subdirectory(3rd)
add_subdirectory(src)
# add_subdirectory(test)

add_executable(tjugo main.cpp)
target_include_directories(tjugo PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(tjugo common icg tcg)
