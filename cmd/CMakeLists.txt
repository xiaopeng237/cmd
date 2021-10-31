project(cmd)
cmake_minimum_required(VERSION 2.8)

#	创建debug版本
set(CMAKE_BUILD_TYPE "Debug")

#   add module directory
add_subdirectory(src)
