cmake_minimum_required(VERSION 3.5)
project(tjugo)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -O3 -g")

enable_testing()


# library in subdirectory
add_subdirectory(3rd)
add_subdirectory(src)
# add_subdirectory(test)


# excutable
add_executable(tjugo main.cpp)

target_include_directories(tjugo
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(tjugo
    PUBLIC
        common
        icg
        tcg
)


# install
install(TARGETS tjugo 3rd common icg tcg
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

file(GLOB_RECURSE demo ${CMAKE_SOURCE_DIR}/demo/*)
install(FILES ${demo}
    TYPE DOC
)
