# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_SOURCE_DIR = /home/ubuntu/desktop/github/abacus/gint

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ubuntu/desktop/github/abacus/gint/build

# Include any dependencies generated for this target.
include module_gint/module_base/CMakeFiles/base.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include module_gint/module_base/CMakeFiles/base.dir/compiler_depend.make

# Include the progress variables for this target.
include module_gint/module_base/CMakeFiles/base.dir/progress.make

# Include the compile flags for this target's objects.
include module_gint/module_base/CMakeFiles/base.dir/flags.make

module_gint/module_base/CMakeFiles/base.dir/tool_quit.cpp.o: module_gint/module_base/CMakeFiles/base.dir/flags.make
module_gint/module_base/CMakeFiles/base.dir/tool_quit.cpp.o: /home/ubuntu/desktop/github/abacus/gint/module_gint/module_base/tool_quit.cpp
module_gint/module_base/CMakeFiles/base.dir/tool_quit.cpp.o: module_gint/module_base/CMakeFiles/base.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/desktop/github/abacus/gint/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object module_gint/module_base/CMakeFiles/base.dir/tool_quit.cpp.o"
	cd /home/ubuntu/desktop/github/abacus/gint/build/module_gint/module_base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT module_gint/module_base/CMakeFiles/base.dir/tool_quit.cpp.o -MF CMakeFiles/base.dir/tool_quit.cpp.o.d -o CMakeFiles/base.dir/tool_quit.cpp.o -c /home/ubuntu/desktop/github/abacus/gint/module_gint/module_base/tool_quit.cpp

module_gint/module_base/CMakeFiles/base.dir/tool_quit.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/base.dir/tool_quit.cpp.i"
	cd /home/ubuntu/desktop/github/abacus/gint/build/module_gint/module_base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/desktop/github/abacus/gint/module_gint/module_base/tool_quit.cpp > CMakeFiles/base.dir/tool_quit.cpp.i

module_gint/module_base/CMakeFiles/base.dir/tool_quit.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/base.dir/tool_quit.cpp.s"
	cd /home/ubuntu/desktop/github/abacus/gint/build/module_gint/module_base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/desktop/github/abacus/gint/module_gint/module_base/tool_quit.cpp -o CMakeFiles/base.dir/tool_quit.cpp.s

module_gint/module_base/CMakeFiles/base.dir/tool_title.cpp.o: module_gint/module_base/CMakeFiles/base.dir/flags.make
module_gint/module_base/CMakeFiles/base.dir/tool_title.cpp.o: /home/ubuntu/desktop/github/abacus/gint/module_gint/module_base/tool_title.cpp
module_gint/module_base/CMakeFiles/base.dir/tool_title.cpp.o: module_gint/module_base/CMakeFiles/base.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/desktop/github/abacus/gint/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object module_gint/module_base/CMakeFiles/base.dir/tool_title.cpp.o"
	cd /home/ubuntu/desktop/github/abacus/gint/build/module_gint/module_base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT module_gint/module_base/CMakeFiles/base.dir/tool_title.cpp.o -MF CMakeFiles/base.dir/tool_title.cpp.o.d -o CMakeFiles/base.dir/tool_title.cpp.o -c /home/ubuntu/desktop/github/abacus/gint/module_gint/module_base/tool_title.cpp

module_gint/module_base/CMakeFiles/base.dir/tool_title.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/base.dir/tool_title.cpp.i"
	cd /home/ubuntu/desktop/github/abacus/gint/build/module_gint/module_base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/desktop/github/abacus/gint/module_gint/module_base/tool_title.cpp > CMakeFiles/base.dir/tool_title.cpp.i

module_gint/module_base/CMakeFiles/base.dir/tool_title.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/base.dir/tool_title.cpp.s"
	cd /home/ubuntu/desktop/github/abacus/gint/build/module_gint/module_base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/desktop/github/abacus/gint/module_gint/module_base/tool_title.cpp -o CMakeFiles/base.dir/tool_title.cpp.s

base: module_gint/module_base/CMakeFiles/base.dir/tool_quit.cpp.o
base: module_gint/module_base/CMakeFiles/base.dir/tool_title.cpp.o
base: module_gint/module_base/CMakeFiles/base.dir/build.make
.PHONY : base

# Rule to build all files generated by this target.
module_gint/module_base/CMakeFiles/base.dir/build: base
.PHONY : module_gint/module_base/CMakeFiles/base.dir/build

module_gint/module_base/CMakeFiles/base.dir/clean:
	cd /home/ubuntu/desktop/github/abacus/gint/build/module_gint/module_base && $(CMAKE_COMMAND) -P CMakeFiles/base.dir/cmake_clean.cmake
.PHONY : module_gint/module_base/CMakeFiles/base.dir/clean

module_gint/module_base/CMakeFiles/base.dir/depend:
	cd /home/ubuntu/desktop/github/abacus/gint/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu/desktop/github/abacus/gint /home/ubuntu/desktop/github/abacus/gint/module_gint/module_base /home/ubuntu/desktop/github/abacus/gint/build /home/ubuntu/desktop/github/abacus/gint/build/module_gint/module_base /home/ubuntu/desktop/github/abacus/gint/build/module_gint/module_base/CMakeFiles/base.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : module_gint/module_base/CMakeFiles/base.dir/depend

