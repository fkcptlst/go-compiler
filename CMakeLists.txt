cmake_minimum_required(VERSION 3.5)
project(tjugo)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -g")

enable_testing()

add_subdirectory(src)
add_subdirectory(test)

add_executable(
  tjugo main.cpp
)

target_link_libraries(tjugo src)
