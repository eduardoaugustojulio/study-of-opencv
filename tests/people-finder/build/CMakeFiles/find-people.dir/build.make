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
CMAKE_SOURCE_DIR = /home/eduardo/Documents/study-of-opencv/tests/people-finder

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/eduardo/Documents/study-of-opencv/tests/people-finder/build

# Include any dependencies generated for this target.
include CMakeFiles/find-people.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/find-people.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/find-people.dir/flags.make

CMakeFiles/find-people.dir/src/main.cpp.o: CMakeFiles/find-people.dir/flags.make
CMakeFiles/find-people.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/eduardo/Documents/study-of-opencv/tests/people-finder/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/find-people.dir/src/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/find-people.dir/src/main.cpp.o -c /home/eduardo/Documents/study-of-opencv/tests/people-finder/src/main.cpp

CMakeFiles/find-people.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/find-people.dir/src/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/eduardo/Documents/study-of-opencv/tests/people-finder/src/main.cpp > CMakeFiles/find-people.dir/src/main.cpp.i

CMakeFiles/find-people.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/find-people.dir/src/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/eduardo/Documents/study-of-opencv/tests/people-finder/src/main.cpp -o CMakeFiles/find-people.dir/src/main.cpp.s

CMakeFiles/find-people.dir/src/main.cpp.o.requires:

.PHONY : CMakeFiles/find-people.dir/src/main.cpp.o.requires

CMakeFiles/find-people.dir/src/main.cpp.o.provides: CMakeFiles/find-people.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/find-people.dir/build.make CMakeFiles/find-people.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/find-people.dir/src/main.cpp.o.provides

CMakeFiles/find-people.dir/src/main.cpp.o.provides.build: CMakeFiles/find-people.dir/src/main.cpp.o


# Object files for target find-people
find__people_OBJECTS = \
"CMakeFiles/find-people.dir/src/main.cpp.o"

# External object files for target find-people
find__people_EXTERNAL_OBJECTS =

find-people: CMakeFiles/find-people.dir/src/main.cpp.o
find-people: CMakeFiles/find-people.dir/build.make
find-people: /usr/local/lib/libopencv_dnn.so.3.3.0
find-people: /usr/local/lib/libopencv_ml.so.3.3.0
find-people: /usr/local/lib/libopencv_objdetect.so.3.3.0
find-people: /usr/local/lib/libopencv_shape.so.3.3.0
find-people: /usr/local/lib/libopencv_stitching.so.3.3.0
find-people: /usr/local/lib/libopencv_superres.so.3.3.0
find-people: /usr/local/lib/libopencv_videostab.so.3.3.0
find-people: /usr/local/lib/libopencv_calib3d.so.3.3.0
find-people: /usr/local/lib/libopencv_features2d.so.3.3.0
find-people: /usr/local/lib/libopencv_flann.so.3.3.0
find-people: /usr/local/lib/libopencv_highgui.so.3.3.0
find-people: /usr/local/lib/libopencv_photo.so.3.3.0
find-people: /usr/local/lib/libopencv_video.so.3.3.0
find-people: /usr/local/lib/libopencv_videoio.so.3.3.0
find-people: /usr/local/lib/libopencv_imgcodecs.so.3.3.0
find-people: /usr/local/lib/libopencv_imgproc.so.3.3.0
find-people: /usr/local/lib/libopencv_core.so.3.3.0
find-people: CMakeFiles/find-people.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/eduardo/Documents/study-of-opencv/tests/people-finder/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable find-people"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/find-people.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/find-people.dir/build: find-people

.PHONY : CMakeFiles/find-people.dir/build

CMakeFiles/find-people.dir/requires: CMakeFiles/find-people.dir/src/main.cpp.o.requires

.PHONY : CMakeFiles/find-people.dir/requires

CMakeFiles/find-people.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/find-people.dir/cmake_clean.cmake
.PHONY : CMakeFiles/find-people.dir/clean

CMakeFiles/find-people.dir/depend:
	cd /home/eduardo/Documents/study-of-opencv/tests/people-finder/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/eduardo/Documents/study-of-opencv/tests/people-finder /home/eduardo/Documents/study-of-opencv/tests/people-finder /home/eduardo/Documents/study-of-opencv/tests/people-finder/build /home/eduardo/Documents/study-of-opencv/tests/people-finder/build /home/eduardo/Documents/study-of-opencv/tests/people-finder/build/CMakeFiles/find-people.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/find-people.dir/depend
