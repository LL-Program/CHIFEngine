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
CMAKE_SOURCE_DIR = /mnt/f/CHIFEngine/CHIF-Engine

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/f/CHIFEngine/CHIF-Engine/build

# Include any dependencies generated for this target.
include src/core/CMakeFiles/core.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/core/CMakeFiles/core.dir/compiler_depend.make

# Include the progress variables for this target.
include src/core/CMakeFiles/core.dir/progress.make

# Include the compile flags for this target's objects.
include src/core/CMakeFiles/core.dir/flags.make

src/core/CMakeFiles/core.dir/imgui/imgui.cpp.o: src/core/CMakeFiles/core.dir/flags.make
src/core/CMakeFiles/core.dir/imgui/imgui.cpp.o: /mnt/f/CHIFEngine/CHIF-Engine/src/core/imgui/imgui.cpp
src/core/CMakeFiles/core.dir/imgui/imgui.cpp.o: src/core/CMakeFiles/core.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/mnt/f/CHIFEngine/CHIF-Engine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/core/CMakeFiles/core.dir/imgui/imgui.cpp.o"
	cd /mnt/f/CHIFEngine/CHIF-Engine/build/src/core && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/core/CMakeFiles/core.dir/imgui/imgui.cpp.o -MF CMakeFiles/core.dir/imgui/imgui.cpp.o.d -o CMakeFiles/core.dir/imgui/imgui.cpp.o -c /mnt/f/CHIFEngine/CHIF-Engine/src/core/imgui/imgui.cpp

src/core/CMakeFiles/core.dir/imgui/imgui.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/core.dir/imgui/imgui.cpp.i"
	cd /mnt/f/CHIFEngine/CHIF-Engine/build/src/core && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/f/CHIFEngine/CHIF-Engine/src/core/imgui/imgui.cpp > CMakeFiles/core.dir/imgui/imgui.cpp.i

src/core/CMakeFiles/core.dir/imgui/imgui.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/core.dir/imgui/imgui.cpp.s"
	cd /mnt/f/CHIFEngine/CHIF-Engine/build/src/core && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/f/CHIFEngine/CHIF-Engine/src/core/imgui/imgui.cpp -o CMakeFiles/core.dir/imgui/imgui.cpp.s

src/core/CMakeFiles/core.dir/imgui/imgui_draw.cpp.o: src/core/CMakeFiles/core.dir/flags.make
src/core/CMakeFiles/core.dir/imgui/imgui_draw.cpp.o: /mnt/f/CHIFEngine/CHIF-Engine/src/core/imgui/imgui_draw.cpp
src/core/CMakeFiles/core.dir/imgui/imgui_draw.cpp.o: src/core/CMakeFiles/core.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/mnt/f/CHIFEngine/CHIF-Engine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/core/CMakeFiles/core.dir/imgui/imgui_draw.cpp.o"
	cd /mnt/f/CHIFEngine/CHIF-Engine/build/src/core && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/core/CMakeFiles/core.dir/imgui/imgui_draw.cpp.o -MF CMakeFiles/core.dir/imgui/imgui_draw.cpp.o.d -o CMakeFiles/core.dir/imgui/imgui_draw.cpp.o -c /mnt/f/CHIFEngine/CHIF-Engine/src/core/imgui/imgui_draw.cpp

src/core/CMakeFiles/core.dir/imgui/imgui_draw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/core.dir/imgui/imgui_draw.cpp.i"
	cd /mnt/f/CHIFEngine/CHIF-Engine/build/src/core && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/f/CHIFEngine/CHIF-Engine/src/core/imgui/imgui_draw.cpp > CMakeFiles/core.dir/imgui/imgui_draw.cpp.i

src/core/CMakeFiles/core.dir/imgui/imgui_draw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/core.dir/imgui/imgui_draw.cpp.s"
	cd /mnt/f/CHIFEngine/CHIF-Engine/build/src/core && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/f/CHIFEngine/CHIF-Engine/src/core/imgui/imgui_draw.cpp -o CMakeFiles/core.dir/imgui/imgui_draw.cpp.s

src/core/CMakeFiles/core.dir/imgui_impl_glfw_gl3.cpp.o: src/core/CMakeFiles/core.dir/flags.make
src/core/CMakeFiles/core.dir/imgui_impl_glfw_gl3.cpp.o: /mnt/f/CHIFEngine/CHIF-Engine/src/core/imgui_impl_glfw_gl3.cpp
src/core/CMakeFiles/core.dir/imgui_impl_glfw_gl3.cpp.o: src/core/CMakeFiles/core.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/mnt/f/CHIFEngine/CHIF-Engine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/core/CMakeFiles/core.dir/imgui_impl_glfw_gl3.cpp.o"
	cd /mnt/f/CHIFEngine/CHIF-Engine/build/src/core && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/core/CMakeFiles/core.dir/imgui_impl_glfw_gl3.cpp.o -MF CMakeFiles/core.dir/imgui_impl_glfw_gl3.cpp.o.d -o CMakeFiles/core.dir/imgui_impl_glfw_gl3.cpp.o -c /mnt/f/CHIFEngine/CHIF-Engine/src/core/imgui_impl_glfw_gl3.cpp

src/core/CMakeFiles/core.dir/imgui_impl_glfw_gl3.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/core.dir/imgui_impl_glfw_gl3.cpp.i"
	cd /mnt/f/CHIFEngine/CHIF-Engine/build/src/core && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/f/CHIFEngine/CHIF-Engine/src/core/imgui_impl_glfw_gl3.cpp > CMakeFiles/core.dir/imgui_impl_glfw_gl3.cpp.i

src/core/CMakeFiles/core.dir/imgui_impl_glfw_gl3.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/core.dir/imgui_impl_glfw_gl3.cpp.s"
	cd /mnt/f/CHIFEngine/CHIF-Engine/build/src/core && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/f/CHIFEngine/CHIF-Engine/src/core/imgui_impl_glfw_gl3.cpp -o CMakeFiles/core.dir/imgui_impl_glfw_gl3.cpp.s

src/core/CMakeFiles/core.dir/main.cpp.o: src/core/CMakeFiles/core.dir/flags.make
src/core/CMakeFiles/core.dir/main.cpp.o: /mnt/f/CHIFEngine/CHIF-Engine/src/core/main.cpp
src/core/CMakeFiles/core.dir/main.cpp.o: src/core/CMakeFiles/core.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/mnt/f/CHIFEngine/CHIF-Engine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/core/CMakeFiles/core.dir/main.cpp.o"
	cd /mnt/f/CHIFEngine/CHIF-Engine/build/src/core && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/core/CMakeFiles/core.dir/main.cpp.o -MF CMakeFiles/core.dir/main.cpp.o.d -o CMakeFiles/core.dir/main.cpp.o -c /mnt/f/CHIFEngine/CHIF-Engine/src/core/main.cpp

src/core/CMakeFiles/core.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/core.dir/main.cpp.i"
	cd /mnt/f/CHIFEngine/CHIF-Engine/build/src/core && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/f/CHIFEngine/CHIF-Engine/src/core/main.cpp > CMakeFiles/core.dir/main.cpp.i

src/core/CMakeFiles/core.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/core.dir/main.cpp.s"
	cd /mnt/f/CHIFEngine/CHIF-Engine/build/src/core && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/f/CHIFEngine/CHIF-Engine/src/core/main.cpp -o CMakeFiles/core.dir/main.cpp.s

# Object files for target core
core_OBJECTS = \
"CMakeFiles/core.dir/imgui/imgui.cpp.o" \
"CMakeFiles/core.dir/imgui/imgui_draw.cpp.o" \
"CMakeFiles/core.dir/imgui_impl_glfw_gl3.cpp.o" \
"CMakeFiles/core.dir/main.cpp.o"

# External object files for target core
core_EXTERNAL_OBJECTS =

src/core/core: src/core/CMakeFiles/core.dir/imgui/imgui.cpp.o
src/core/core: src/core/CMakeFiles/core.dir/imgui/imgui_draw.cpp.o
src/core/core: src/core/CMakeFiles/core.dir/imgui_impl_glfw_gl3.cpp.o
src/core/core: src/core/CMakeFiles/core.dir/main.cpp.o
src/core/core: src/core/CMakeFiles/core.dir/build.make
src/core/core: /usr/lib/x86_64-linux-gnu/libGL.so
src/core/core: /usr/lib/x86_64-linux-gnu/libGLU.so
src/core/core: /usr/lib/x86_64-linux-gnu/libglfw.so
src/core/core: /usr/lib/x86_64-linux-gnu/libGLEW.so
src/core/core: src/core/CMakeFiles/core.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/mnt/f/CHIFEngine/CHIF-Engine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable core"
	cd /mnt/f/CHIFEngine/CHIF-Engine/build/src/core && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/core.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/core/CMakeFiles/core.dir/build: src/core/core
.PHONY : src/core/CMakeFiles/core.dir/build

src/core/CMakeFiles/core.dir/clean:
	cd /mnt/f/CHIFEngine/CHIF-Engine/build/src/core && $(CMAKE_COMMAND) -P CMakeFiles/core.dir/cmake_clean.cmake
.PHONY : src/core/CMakeFiles/core.dir/clean

src/core/CMakeFiles/core.dir/depend:
	cd /mnt/f/CHIFEngine/CHIF-Engine/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/f/CHIFEngine/CHIF-Engine /mnt/f/CHIFEngine/CHIF-Engine/src/core /mnt/f/CHIFEngine/CHIF-Engine/build /mnt/f/CHIFEngine/CHIF-Engine/build/src/core /mnt/f/CHIFEngine/CHIF-Engine/build/src/core/CMakeFiles/core.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/core/CMakeFiles/core.dir/depend

