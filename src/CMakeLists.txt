file(GLOB_RECURSE SRC_LIB_SRC *.cpp)

add_library(src
	${SRC_LIB_SRC}
)

target_include_directories(src PUBLIC ${CMAKE_SOURCE_DIR}/include)


