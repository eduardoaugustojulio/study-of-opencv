# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/eduardo/Documents/study-of-opencv/tests/countours

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/eduardo/Documents/study-of-opencv/tests/countours/build

# Include any dependencies generated for this target.
include CMakeFiles/segments-objects.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/segments-objects.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/segments-objects.dir/flags.make

CMakeFiles/segments-objects.dir/src/main.cpp.o: CMakeFiles/segments-objects.dir/flags.make
CMakeFiles/segments-objects.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/eduardo/Documents/study-of-opencv/tests/countours/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/segments-objects.dir/src/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/segments-objects.dir/src/main.cpp.o -c /home/eduardo/Documents/study-of-opencv/tests/countours/src/main.cpp

CMakeFiles/segments-objects.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/segments-objects.dir/src/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/eduardo/Documents/study-of-opencv/tests/countours/src/main.cpp > CMakeFiles/segments-objects.dir/src/main.cpp.i

CMakeFiles/segments-objects.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/segments-objects.dir/src/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/eduardo/Documents/study-of-opencv/tests/countours/src/main.cpp -o CMakeFiles/segments-objects.dir/src/main.cpp.s

CMakeFiles/segments-objects.dir/src/main.cpp.o.requires:

.PHONY : CMakeFiles/segments-objects.dir/src/main.cpp.o.requires

CMakeFiles/segments-objects.dir/src/main.cpp.o.provides: CMakeFiles/segments-objects.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/segments-objects.dir/build.make CMakeFiles/segments-objects.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/segments-objects.dir/src/main.cpp.o.provides

CMakeFiles/segments-objects.dir/src/main.cpp.o.provides.build: CMakeFiles/segments-objects.dir/src/main.cpp.o


# Object files for target segments-objects
segments__objects_OBJECTS = \
"CMakeFiles/segments-objects.dir/src/main.cpp.o"

# External object files for target segments-objects
segments__objects_EXTERNAL_OBJECTS =

segments-objects: CMakeFiles/segments-objects.dir/src/main.cpp.o
segments-objects: CMakeFiles/segments-objects.dir/build.make
segments-objects: /usr/local/lib/libopencv_dnn.so.3.3.0
segments-objects: /usr/local/lib/libopencv_ml.so.3.3.0
segments-objects: /usr/local/lib/libopencv_objdetect.so.3.3.0
segments-objects: /usr/local/lib/libopencv_shape.so.3.3.0
segments-objects: /usr/local/lib/libopencv_stitching.so.3.3.0
segments-objects: /usr/local/lib/libopencv_superres.so.3.3.0
segments-objects: /usr/local/lib/libopencv_videostab.so.3.3.0
segments-objects: /usr/local/lib/libopencv_calib3d.so.3.3.0
segments-objects: /usr/local/lib/libopencv_features2d.so.3.3.0
segments-objects: /usr/local/lib/libopencv_flann.so.3.3.0
segments-objects: /usr/local/lib/libopencv_highgui.so.3.3.0
segments-objects: /usr/local/lib/libopencv_photo.so.3.3.0
segments-objects: /usr/local/lib/libopencv_video.so.3.3.0
segments-objects: /usr/local/lib/libopencv_videoio.so.3.3.0
segments-objects: /usr/local/lib/libopencv_imgcodecs.so.3.3.0
segments-objects: /usr/local/lib/libopencv_imgproc.so.3.3.0
segments-objects: /usr/local/lib/libopencv_core.so.3.3.0
segments-objects: CMakeFiles/segments-objects.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/eduardo/Documents/study-of-opencv/tests/countours/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable segments-objects"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/segments-objects.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/segments-objects.dir/build: segments-objects

.PHONY : CMakeFiles/segments-objects.dir/build

CMakeFiles/segments-objects.dir/requires: CMakeFiles/segments-objects.dir/src/main.cpp.o.requires

.PHONY : CMakeFiles/segments-objects.dir/requires

CMakeFiles/segments-objects.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/segments-objects.dir/cmake_clean.cmake
.PHONY : CMakeFiles/segments-objects.dir/clean

CMakeFiles/segments-objects.dir/depend:
	cd /home/eduardo/Documents/study-of-opencv/tests/countours/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/eduardo/Documents/study-of-opencv/tests/countours /home/eduardo/Documents/study-of-opencv/tests/countours /home/eduardo/Documents/study-of-opencv/tests/countours/build /home/eduardo/Documents/study-of-opencv/tests/countours/build /home/eduardo/Documents/study-of-opencv/tests/countours/build/CMakeFiles/segments-objects.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/segments-objects.dir/depend
