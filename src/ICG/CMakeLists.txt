# g4 --> cpp
set(ANTLR4CPP_JAR_LOCATION ${CMAKE_SOURCE_DIR}/3rd/antlr-4.9.3-complete.jar)

file(GLOB grammar_g4
    ${CMAKE_SOURCE_DIR}/grammar/*.g4
)

set(ANTLR4_FLAGS -Dlanguage=Cpp)
execute_process(
    COMMAND java -jar "${ANTLR4CPP_JAR_LOCATION}" ${ANTLR4_FLAGS} ${grammar_g4} -o ${CMAKE_SOURCE_DIR}/grammar/gen
)

set(ANTLR4_FLAGS -Dlanguage=Java)
execute_process(
    COMMAND java -jar "${ANTLR4CPP_JAR_LOCATION}" ${ANTLR4_FLAGS} ${grammar_g4} -o ${CMAKE_SOURCE_DIR}/grammar/java
)

# cmake
file(GLOB_RECURSE src_lib_gen ${CMAKE_SOURCE_DIR}/grammar/gen/*.cpp)
file(GLOB_RECURSE src_lib_icg *.cpp)

add_library(lib_gen ${src_lib_gen})

add_library(lib_icg ${src_lib_icg})
add_dependencies(lib_icg lib_gen)

add_library(icg
    ${src_lib_gen}
    ${src_lib_icg}
)

target_include_directories(icg PUBLIC
    ${CMAKE_SOURCE_DIR}/3rd/antlr4-runtime/src
    ${CMAKE_SOURCE_DIR}/grammar/gen
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(icg 3rd common)
