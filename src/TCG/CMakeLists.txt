file(GLOB_RECURSE src_lib_tcg *.cpp)

add_library(tcg
	${src_lib_tcg}
)

target_include_directories(tcg PUBLIC ${CMAKE_SOURCE_DIR}/include)
