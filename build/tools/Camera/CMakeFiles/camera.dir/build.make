# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_SOURCE_DIR = /home/lifu/Documents/DBdriver/pq

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lifu/Documents/DBdriver/pq/build

# Include any dependencies generated for this target.
include tools/Camera/CMakeFiles/camera.dir/depend.make

# Include the progress variables for this target.
include tools/Camera/CMakeFiles/camera.dir/progress.make

# Include the compile flags for this target's objects.
include tools/Camera/CMakeFiles/camera.dir/flags.make

tools/Camera/CMakeFiles/camera.dir/main.cpp.o: tools/Camera/CMakeFiles/camera.dir/flags.make
tools/Camera/CMakeFiles/camera.dir/main.cpp.o: ../tools/Camera/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/lifu/Documents/DBdriver/pq/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object tools/Camera/CMakeFiles/camera.dir/main.cpp.o"
	cd /home/lifu/Documents/DBdriver/pq/build/tools/Camera && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/camera.dir/main.cpp.o -c /home/lifu/Documents/DBdriver/pq/tools/Camera/main.cpp

tools/Camera/CMakeFiles/camera.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/camera.dir/main.cpp.i"
	cd /home/lifu/Documents/DBdriver/pq/build/tools/Camera && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/lifu/Documents/DBdriver/pq/tools/Camera/main.cpp > CMakeFiles/camera.dir/main.cpp.i

tools/Camera/CMakeFiles/camera.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/camera.dir/main.cpp.s"
	cd /home/lifu/Documents/DBdriver/pq/build/tools/Camera && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/lifu/Documents/DBdriver/pq/tools/Camera/main.cpp -o CMakeFiles/camera.dir/main.cpp.s

tools/Camera/CMakeFiles/camera.dir/main.cpp.o.requires:
.PHONY : tools/Camera/CMakeFiles/camera.dir/main.cpp.o.requires

tools/Camera/CMakeFiles/camera.dir/main.cpp.o.provides: tools/Camera/CMakeFiles/camera.dir/main.cpp.o.requires
	$(MAKE) -f tools/Camera/CMakeFiles/camera.dir/build.make tools/Camera/CMakeFiles/camera.dir/main.cpp.o.provides.build
.PHONY : tools/Camera/CMakeFiles/camera.dir/main.cpp.o.provides

tools/Camera/CMakeFiles/camera.dir/main.cpp.o.provides.build: tools/Camera/CMakeFiles/camera.dir/main.cpp.o

# Object files for target camera
camera_OBJECTS = \
"CMakeFiles/camera.dir/main.cpp.o"

# External object files for target camera
camera_EXTERNAL_OBJECTS =

../bin/rtabmap-camera: tools/Camera/CMakeFiles/camera.dir/main.cpp.o
../bin/rtabmap-camera: tools/Camera/CMakeFiles/camera.dir/build.make
../bin/rtabmap-camera: ../bin/librtabmap_core.so.0.11.5
../bin/rtabmap-camera: ../bin/librtabmap_utilite.so.0.11.5
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libopencv_videostab.so.2.4.8
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libopencv_video.so.2.4.8
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libopencv_ts.so.2.4.8
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libopencv_superres.so.2.4.8
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libopencv_stitching.so.2.4.8
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libopencv_photo.so.2.4.8
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libopencv_ocl.so.2.4.8
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libopencv_objdetect.so.2.4.8
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libopencv_ml.so.2.4.8
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libopencv_legacy.so.2.4.8
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so.2.4.8
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libopencv_highgui.so.2.4.8
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libopencv_gpu.so.2.4.8
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libopencv_flann.so.2.4.8
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libopencv_features2d.so.2.4.8
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libopencv_core.so.2.4.8
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libopencv_contrib.so.2.4.8
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libopencv_calib3d.so.2.4.8
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libboost_system.so
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libboost_thread.so
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libboost_date_time.so
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libboost_iostreams.so
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libboost_serialization.so
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libpthread.so
../bin/rtabmap-camera: /usr/lib/libpcl_common.so
../bin/rtabmap-camera: /usr/lib/libpcl_octree.so
../bin/rtabmap-camera: /usr/lib/libOpenNI.so
../bin/rtabmap-camera: /usr/lib/libvtkCommon.so.5.8.0
../bin/rtabmap-camera: /usr/lib/libvtkRendering.so.5.8.0
../bin/rtabmap-camera: /usr/lib/libvtkHybrid.so.5.8.0
../bin/rtabmap-camera: /usr/lib/libvtkCharts.so.5.8.0
../bin/rtabmap-camera: /usr/lib/libpcl_io.so
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libflann_cpp_s.a
../bin/rtabmap-camera: /usr/lib/libpcl_kdtree.so
../bin/rtabmap-camera: /usr/lib/libpcl_search.so
../bin/rtabmap-camera: /usr/lib/libpcl_sample_consensus.so
../bin/rtabmap-camera: /usr/lib/libpcl_filters.so
../bin/rtabmap-camera: /usr/lib/libpcl_features.so
../bin/rtabmap-camera: /usr/lib/libpcl_keypoints.so
../bin/rtabmap-camera: /usr/lib/libpcl_segmentation.so
../bin/rtabmap-camera: /usr/lib/libpcl_visualization.so
../bin/rtabmap-camera: /usr/lib/libpcl_outofcore.so
../bin/rtabmap-camera: /usr/lib/libpcl_registration.so
../bin/rtabmap-camera: /usr/lib/libpcl_recognition.so
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libqhull.so
../bin/rtabmap-camera: /usr/lib/libpcl_surface.so
../bin/rtabmap-camera: /usr/lib/libpcl_people.so
../bin/rtabmap-camera: /usr/lib/libpcl_tracking.so
../bin/rtabmap-camera: /usr/lib/libpcl_apps.so
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libboost_system.so
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libboost_thread.so
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libboost_date_time.so
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libboost_iostreams.so
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libboost_serialization.so
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libpthread.so
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libqhull.so
../bin/rtabmap-camera: /usr/lib/libOpenNI.so
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libflann_cpp_s.a
../bin/rtabmap-camera: /usr/lib/libvtkCommon.so.5.8.0
../bin/rtabmap-camera: /usr/lib/libvtkRendering.so.5.8.0
../bin/rtabmap-camera: /usr/lib/libvtkHybrid.so.5.8.0
../bin/rtabmap-camera: /usr/lib/libvtkCharts.so.5.8.0
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libopencv_photo.so.2.4.8
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libopencv_legacy.so.2.4.8
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libopencv_video.so.2.4.8
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libopencv_objdetect.so.2.4.8
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libopencv_ml.so.2.4.8
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libopencv_calib3d.so.2.4.8
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libopencv_features2d.so.2.4.8
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libopencv_highgui.so.2.4.8
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so.2.4.8
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libopencv_flann.so.2.4.8
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libopencv_core.so.2.4.8
../bin/rtabmap-camera: /usr/lib/libvtkViews.so.5.8.0
../bin/rtabmap-camera: /usr/lib/libvtkInfovis.so.5.8.0
../bin/rtabmap-camera: /usr/lib/libvtkWidgets.so.5.8.0
../bin/rtabmap-camera: /usr/lib/libvtkHybrid.so.5.8.0
../bin/rtabmap-camera: /usr/lib/libvtkParallel.so.5.8.0
../bin/rtabmap-camera: /usr/lib/libvtkVolumeRendering.so.5.8.0
../bin/rtabmap-camera: /usr/lib/libvtkRendering.so.5.8.0
../bin/rtabmap-camera: /usr/lib/libvtkGraphics.so.5.8.0
../bin/rtabmap-camera: /usr/lib/libvtkImaging.so.5.8.0
../bin/rtabmap-camera: /usr/lib/libvtkIO.so.5.8.0
../bin/rtabmap-camera: /usr/lib/libvtkFiltering.so.5.8.0
../bin/rtabmap-camera: /usr/lib/libvtkCommon.so.5.8.0
../bin/rtabmap-camera: /usr/lib/libvtksys.so.5.8.0
../bin/rtabmap-camera: /usr/lib/libpcl_common.so
../bin/rtabmap-camera: /usr/lib/libpcl_octree.so
../bin/rtabmap-camera: /usr/lib/libpcl_io.so
../bin/rtabmap-camera: /usr/lib/libpcl_kdtree.so
../bin/rtabmap-camera: /usr/lib/libpcl_search.so
../bin/rtabmap-camera: /usr/lib/libpcl_sample_consensus.so
../bin/rtabmap-camera: /usr/lib/libpcl_filters.so
../bin/rtabmap-camera: /usr/lib/libpcl_features.so
../bin/rtabmap-camera: /usr/lib/libpcl_keypoints.so
../bin/rtabmap-camera: /usr/lib/libpcl_segmentation.so
../bin/rtabmap-camera: /usr/lib/libpcl_visualization.so
../bin/rtabmap-camera: /usr/lib/libpcl_outofcore.so
../bin/rtabmap-camera: /usr/lib/libpcl_registration.so
../bin/rtabmap-camera: /usr/lib/libpcl_recognition.so
../bin/rtabmap-camera: /usr/lib/libpcl_surface.so
../bin/rtabmap-camera: /usr/lib/libpcl_people.so
../bin/rtabmap-camera: /usr/lib/libpcl_tracking.so
../bin/rtabmap-camera: /usr/lib/libpcl_apps.so
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libpthread.so
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libz.so
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libfreenect.so
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libfreenect_sync.so
../bin/rtabmap-camera: /usr/lib/libOpenNI2.so
../bin/rtabmap-camera: /usr/lib/x86_64-linux-gnu/libdc1394.so
../bin/rtabmap-camera: tools/Camera/CMakeFiles/camera.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../../../bin/rtabmap-camera"
	cd /home/lifu/Documents/DBdriver/pq/build/tools/Camera && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/camera.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tools/Camera/CMakeFiles/camera.dir/build: ../bin/rtabmap-camera
.PHONY : tools/Camera/CMakeFiles/camera.dir/build

tools/Camera/CMakeFiles/camera.dir/requires: tools/Camera/CMakeFiles/camera.dir/main.cpp.o.requires
.PHONY : tools/Camera/CMakeFiles/camera.dir/requires

tools/Camera/CMakeFiles/camera.dir/clean:
	cd /home/lifu/Documents/DBdriver/pq/build/tools/Camera && $(CMAKE_COMMAND) -P CMakeFiles/camera.dir/cmake_clean.cmake
.PHONY : tools/Camera/CMakeFiles/camera.dir/clean

tools/Camera/CMakeFiles/camera.dir/depend:
	cd /home/lifu/Documents/DBdriver/pq/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lifu/Documents/DBdriver/pq /home/lifu/Documents/DBdriver/pq/tools/Camera /home/lifu/Documents/DBdriver/pq/build /home/lifu/Documents/DBdriver/pq/build/tools/Camera /home/lifu/Documents/DBdriver/pq/build/tools/Camera/CMakeFiles/camera.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tools/Camera/CMakeFiles/camera.dir/depend
