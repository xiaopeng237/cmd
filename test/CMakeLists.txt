project(test)
cmake_minimum_required(VERSION 2.8)

#	创建debug版本
set(CMAKE_BUILD_TYPE "Debug")

#   add module directory
add_subdirectory(src)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(tools /home/vmuser/arm-fsl-linux-gnueabi)
set(CMAKE_C_COMPILE_FEATURES ${tools}/bin/arm-fsl-linux-gnueabi-gcc)
set(CMAKE_CXX_COMPILE_FEATURES ${tools}/bin/arm-fsl-linux-gnueabi-g++)