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
CMAKE_SOURCE_DIR = /home/kukarekus/proga/projects/cpp/2024/MyLang

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kukarekus/proga/projects/cpp/2024/MyLang/.build

# Include any dependencies generated for this target.
include CMakeFiles/genDFA.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/genDFA.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/genDFA.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/genDFA.dir/flags.make

CMakeFiles/genDFA.dir/src/RE/genDFA.o: CMakeFiles/genDFA.dir/flags.make
CMakeFiles/genDFA.dir/src/RE/genDFA.o: /home/kukarekus/proga/projects/cpp/2024/MyLang/src/RE/genDFA.cpp
CMakeFiles/genDFA.dir/src/RE/genDFA.o: CMakeFiles/genDFA.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kukarekus/proga/projects/cpp/2024/MyLang/.build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/genDFA.dir/src/RE/genDFA.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/genDFA.dir/src/RE/genDFA.o -MF CMakeFiles/genDFA.dir/src/RE/genDFA.o.d -o CMakeFiles/genDFA.dir/src/RE/genDFA.o -c /home/kukarekus/proga/projects/cpp/2024/MyLang/src/RE/genDFA.cpp

CMakeFiles/genDFA.dir/src/RE/genDFA.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/genDFA.dir/src/RE/genDFA.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kukarekus/proga/projects/cpp/2024/MyLang/src/RE/genDFA.cpp > CMakeFiles/genDFA.dir/src/RE/genDFA.i

CMakeFiles/genDFA.dir/src/RE/genDFA.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/genDFA.dir/src/RE/genDFA.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kukarekus/proga/projects/cpp/2024/MyLang/src/RE/genDFA.cpp -o CMakeFiles/genDFA.dir/src/RE/genDFA.s

# Object files for target genDFA
genDFA_OBJECTS = \
"CMakeFiles/genDFA.dir/src/RE/genDFA.o"

# External object files for target genDFA
genDFA_EXTERNAL_OBJECTS =

/home/kukarekus/proga/projects/cpp/2024/MyLang/bin/genDFA: CMakeFiles/genDFA.dir/src/RE/genDFA.o
/home/kukarekus/proga/projects/cpp/2024/MyLang/bin/genDFA: CMakeFiles/genDFA.dir/build.make
/home/kukarekus/proga/projects/cpp/2024/MyLang/bin/genDFA: CMakeFiles/genDFA.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/kukarekus/proga/projects/cpp/2024/MyLang/.build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable /home/kukarekus/proga/projects/cpp/2024/MyLang/bin/genDFA"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/genDFA.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/genDFA.dir/build: /home/kukarekus/proga/projects/cpp/2024/MyLang/bin/genDFA
.PHONY : CMakeFiles/genDFA.dir/build

CMakeFiles/genDFA.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/genDFA.dir/cmake_clean.cmake
.PHONY : CMakeFiles/genDFA.dir/clean

CMakeFiles/genDFA.dir/depend:
	cd /home/kukarekus/proga/projects/cpp/2024/MyLang/.build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kukarekus/proga/projects/cpp/2024/MyLang /home/kukarekus/proga/projects/cpp/2024/MyLang /home/kukarekus/proga/projects/cpp/2024/MyLang/.build /home/kukarekus/proga/projects/cpp/2024/MyLang/.build /home/kukarekus/proga/projects/cpp/2024/MyLang/.build/CMakeFiles/genDFA.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/genDFA.dir/depend

