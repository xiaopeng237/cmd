# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/vmuser/git/cmd

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/vmuser/git/cmd/build

# Include any dependencies generated for this target.
include src/CMakeFiles/cmd.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/cmd.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/cmd.dir/flags.make

src/CMakeFiles/cmd.dir/cmd.c.o: src/CMakeFiles/cmd.dir/flags.make
src/CMakeFiles/cmd.dir/cmd.c.o: ../src/cmd.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vmuser/git/cmd/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/CMakeFiles/cmd.dir/cmd.c.o"
	cd /home/vmuser/git/cmd/build/src && /home/vmuser/arm-fsl-linux-gnueabi/bin/arm-fsl-linux-gnueabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/cmd.dir/cmd.c.o   -c /home/vmuser/git/cmd/src/cmd.c

src/CMakeFiles/cmd.dir/cmd.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cmd.dir/cmd.c.i"
	cd /home/vmuser/git/cmd/build/src && /home/vmuser/arm-fsl-linux-gnueabi/bin/arm-fsl-linux-gnueabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/vmuser/git/cmd/src/cmd.c > CMakeFiles/cmd.dir/cmd.c.i

src/CMakeFiles/cmd.dir/cmd.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cmd.dir/cmd.c.s"
	cd /home/vmuser/git/cmd/build/src && /home/vmuser/arm-fsl-linux-gnueabi/bin/arm-fsl-linux-gnueabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/vmuser/git/cmd/src/cmd.c -o CMakeFiles/cmd.dir/cmd.c.s

# Object files for target cmd
cmd_OBJECTS = \
"CMakeFiles/cmd.dir/cmd.c.o"

# External object files for target cmd
cmd_EXTERNAL_OBJECTS =

../bin/cmd: src/CMakeFiles/cmd.dir/cmd.c.o
../bin/cmd: src/CMakeFiles/cmd.dir/build.make
../bin/cmd: src/CMakeFiles/cmd.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/vmuser/git/cmd/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable ../../bin/cmd"
	cd /home/vmuser/git/cmd/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cmd.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/cmd.dir/build: ../bin/cmd

.PHONY : src/CMakeFiles/cmd.dir/build

src/CMakeFiles/cmd.dir/clean:
	cd /home/vmuser/git/cmd/build/src && $(CMAKE_COMMAND) -P CMakeFiles/cmd.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/cmd.dir/clean

src/CMakeFiles/cmd.dir/depend:
	cd /home/vmuser/git/cmd/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vmuser/git/cmd /home/vmuser/git/cmd/src /home/vmuser/git/cmd/build /home/vmuser/git/cmd/build/src /home/vmuser/git/cmd/build/src/CMakeFiles/cmd.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/cmd.dir/depend
