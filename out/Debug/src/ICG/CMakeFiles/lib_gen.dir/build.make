# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/out/Debug

# Include any dependencies generated for this target.
include src/ICG/CMakeFiles/lib_gen.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/ICG/CMakeFiles/lib_gen.dir/compiler_depend.make

# Include the progress variables for this target.
include src/ICG/CMakeFiles/lib_gen.dir/progress.make

# Include the compile flags for this target's objects.
include src/ICG/CMakeFiles/lib_gen.dir/flags.make

src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoLexer.cpp.o: src/ICG/CMakeFiles/lib_gen.dir/flags.make
src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoLexer.cpp.o: /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/grammar/gen/GoLexer.cpp
src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoLexer.cpp.o: src/ICG/CMakeFiles/lib_gen.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/out/Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoLexer.cpp.o"
	cd /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/out/Debug/src/ICG && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoLexer.cpp.o -MF CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoLexer.cpp.o.d -o CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoLexer.cpp.o -c /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/grammar/gen/GoLexer.cpp

src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoLexer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoLexer.cpp.i"
	cd /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/out/Debug/src/ICG && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/grammar/gen/GoLexer.cpp > CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoLexer.cpp.i

src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoLexer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoLexer.cpp.s"
	cd /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/out/Debug/src/ICG && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/grammar/gen/GoLexer.cpp -o CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoLexer.cpp.s

src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParser.cpp.o: src/ICG/CMakeFiles/lib_gen.dir/flags.make
src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParser.cpp.o: /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/grammar/gen/GoParser.cpp
src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParser.cpp.o: src/ICG/CMakeFiles/lib_gen.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/out/Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParser.cpp.o"
	cd /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/out/Debug/src/ICG && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParser.cpp.o -MF CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParser.cpp.o.d -o CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParser.cpp.o -c /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/grammar/gen/GoParser.cpp

src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParser.cpp.i"
	cd /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/out/Debug/src/ICG && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/grammar/gen/GoParser.cpp > CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParser.cpp.i

src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParser.cpp.s"
	cd /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/out/Debug/src/ICG && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/grammar/gen/GoParser.cpp -o CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParser.cpp.s

src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParserBaseListener.cpp.o: src/ICG/CMakeFiles/lib_gen.dir/flags.make
src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParserBaseListener.cpp.o: /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/grammar/gen/GoParserBaseListener.cpp
src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParserBaseListener.cpp.o: src/ICG/CMakeFiles/lib_gen.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/out/Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParserBaseListener.cpp.o"
	cd /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/out/Debug/src/ICG && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParserBaseListener.cpp.o -MF CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParserBaseListener.cpp.o.d -o CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParserBaseListener.cpp.o -c /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/grammar/gen/GoParserBaseListener.cpp

src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParserBaseListener.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParserBaseListener.cpp.i"
	cd /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/out/Debug/src/ICG && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/grammar/gen/GoParserBaseListener.cpp > CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParserBaseListener.cpp.i

src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParserBaseListener.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParserBaseListener.cpp.s"
	cd /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/out/Debug/src/ICG && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/grammar/gen/GoParserBaseListener.cpp -o CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParserBaseListener.cpp.s

src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParserListener.cpp.o: src/ICG/CMakeFiles/lib_gen.dir/flags.make
src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParserListener.cpp.o: /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/grammar/gen/GoParserListener.cpp
src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParserListener.cpp.o: src/ICG/CMakeFiles/lib_gen.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/out/Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParserListener.cpp.o"
	cd /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/out/Debug/src/ICG && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParserListener.cpp.o -MF CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParserListener.cpp.o.d -o CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParserListener.cpp.o -c /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/grammar/gen/GoParserListener.cpp

src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParserListener.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParserListener.cpp.i"
	cd /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/out/Debug/src/ICG && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/grammar/gen/GoParserListener.cpp > CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParserListener.cpp.i

src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParserListener.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParserListener.cpp.s"
	cd /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/out/Debug/src/ICG && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/grammar/gen/GoParserListener.cpp -o CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParserListener.cpp.s

# Object files for target lib_gen
lib_gen_OBJECTS = \
"CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoLexer.cpp.o" \
"CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParser.cpp.o" \
"CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParserBaseListener.cpp.o" \
"CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParserListener.cpp.o"

# External object files for target lib_gen
lib_gen_EXTERNAL_OBJECTS =

src/ICG/liblib_gen.a: src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoLexer.cpp.o
src/ICG/liblib_gen.a: src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParser.cpp.o
src/ICG/liblib_gen.a: src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParserBaseListener.cpp.o
src/ICG/liblib_gen.a: src/ICG/CMakeFiles/lib_gen.dir/__/__/grammar/gen/GoParserListener.cpp.o
src/ICG/liblib_gen.a: src/ICG/CMakeFiles/lib_gen.dir/build.make
src/ICG/liblib_gen.a: src/ICG/CMakeFiles/lib_gen.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/out/Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX static library liblib_gen.a"
	cd /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/out/Debug/src/ICG && $(CMAKE_COMMAND) -P CMakeFiles/lib_gen.dir/cmake_clean_target.cmake
	cd /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/out/Debug/src/ICG && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lib_gen.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/ICG/CMakeFiles/lib_gen.dir/build: src/ICG/liblib_gen.a
.PHONY : src/ICG/CMakeFiles/lib_gen.dir/build

src/ICG/CMakeFiles/lib_gen.dir/clean:
	cd /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/out/Debug/src/ICG && $(CMAKE_COMMAND) -P CMakeFiles/lib_gen.dir/cmake_clean.cmake
.PHONY : src/ICG/CMakeFiles/lib_gen.dir/clean

src/ICG/CMakeFiles/lib_gen.dir/depend:
	cd /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/out/Debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/src/ICG /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/out/Debug /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/out/Debug/src/ICG /mnt/win/E/Christopher/TJU/20231/编译原理/Part3/go-compiler_cpp/out/Debug/src/ICG/CMakeFiles/lib_gen.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/ICG/CMakeFiles/lib_gen.dir/depend

