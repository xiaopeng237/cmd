project(spiCtrl)
cmake_minimum_required(VERSION 2.8)

set (SRC_LIST ${PROJECT_SOURCE_DIR}/src/spiCtrl.c)

add_library (spiCtrl_shared SHARED ${SRC_LIST})
add_library (spiCtrl_static STATIC ${SRC_LIST})

set_target_properties (spiCtrl_shared PROPERTIES OUTPUT_NAME "spiCtrl")
set_target_properties (spiCtrl_static PROPERTIES OUTPUT_NAME "spiCtrl")

set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

set(TOOLCHAIN_DIR "/home/vmuser/arm-fsl-linux-gnueabi")
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/bin/arm-fsl-linux-gnueabi-g++)
set(CMAKE_C_COMPILER   ${TOOLCHAIN_DIR}/bin/arm-fsl-linux-gnueabi-gcc)
set(GNU_FLAGS "-O2 -g")
set(CMAKE_CXX_FLAGS "${GNU_FLAGS} ")
set(CMAKE_C_FLAGS "${GNU_FLAGS}  ")