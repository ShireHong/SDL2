# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.15

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\cmake\bin\cmake.exe

# The command to remove a file.
RM = C:\cmake\bin\cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\git_hub\SDL2\mixer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\git_hub\SDL2\mixer\build

# Include any dependencies generated for this target.
include CMakeFiles/mixer.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/mixer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mixer.dir/flags.make

CMakeFiles/mixer.dir/src/main.cpp.obj: CMakeFiles/mixer.dir/flags.make
CMakeFiles/mixer.dir/src/main.cpp.obj: CMakeFiles/mixer.dir/includes_CXX.rsp
CMakeFiles/mixer.dir/src/main.cpp.obj: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\git_hub\SDL2\mixer\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/mixer.dir/src/main.cpp.obj"
	C:\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\mixer.dir\src\main.cpp.obj -c D:\git_hub\SDL2\mixer\src\main.cpp

CMakeFiles/mixer.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mixer.dir/src/main.cpp.i"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\git_hub\SDL2\mixer\src\main.cpp > CMakeFiles\mixer.dir\src\main.cpp.i

CMakeFiles/mixer.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mixer.dir/src/main.cpp.s"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\git_hub\SDL2\mixer\src\main.cpp -o CMakeFiles\mixer.dir\src\main.cpp.s

# Object files for target mixer
mixer_OBJECTS = \
"CMakeFiles/mixer.dir/src/main.cpp.obj"

# External object files for target mixer
mixer_EXTERNAL_OBJECTS =

mixer.exe: CMakeFiles/mixer.dir/src/main.cpp.obj
mixer.exe: CMakeFiles/mixer.dir/build.make
mixer.exe: C:/MinGW/lib/libSDL2main.a
mixer.exe: C:/MinGW/lib/libSDL2.dll.a
mixer.exe: C:/MinGW/lib/libSDL2_image.dll.a
mixer.exe: C:/MinGW/lib/libSDL2_ttf.dll.a
mixer.exe: C:/MinGW/lib/libSDL2_mixer.dll.a
mixer.exe: CMakeFiles/mixer.dir/linklibs.rsp
mixer.exe: CMakeFiles/mixer.dir/objects1.rsp
mixer.exe: CMakeFiles/mixer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\git_hub\SDL2\mixer\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable mixer.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\mixer.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/mixer.dir/build: mixer.exe

.PHONY : CMakeFiles/mixer.dir/build

CMakeFiles/mixer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\mixer.dir\cmake_clean.cmake
.PHONY : CMakeFiles/mixer.dir/clean

CMakeFiles/mixer.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\git_hub\SDL2\mixer D:\git_hub\SDL2\mixer D:\git_hub\SDL2\mixer\build D:\git_hub\SDL2\mixer\build D:\git_hub\SDL2\mixer\build\CMakeFiles\mixer.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/mixer.dir/depend
