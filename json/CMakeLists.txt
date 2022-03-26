cmake_minimum_required(VERSION 3.5)

project(main)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -g -w")


# g4 --> cpp
set(ANTLR4CPP_JAR_LOCATION ${PROJECT_SOURCE_DIR}/../3rd/antlr-4.9.3-complete.jar)

file(GLOB grammar_g4
    ${PROJECT_SOURCE_DIR}/grammar/*.g4
)


set(ANTLR4_FLAGS -Dlanguage=Cpp)
execute_process(
    COMMAND java -jar "${ANTLR4CPP_JAR_LOCATION}" ${ANTLR4_FLAGS} ${grammar_g4} -o ${PROJECT_SOURCE_DIR}/grammar/gen
)


# include
include_directories(
    ${PROJECT_SOURCE_DIR}/../3rd/antlr4-runtime/src
    ${PROJECT_SOURCE_DIR}/grammar/gen
    ${PROJECT_SOURCE_DIR}/include
)


# cpp
file(GLOB_RECURSE antlr4_runtime
    ${PROJECT_SOURCE_DIR}/../3rd/antlr4-runtime/src/*.cpp
)

file(GLOB_RECURSE grammar_cpp ${PROJECT_SOURCE_DIR}/grammar/gen/*.cpp)

file(GLOB_RECURSE sourcefiles ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(main ${sourcefiles} ${antlr4_runtime} ${grammar_cpp})
