project(iicCtrl)
cmake_minimum_required(VERSION 2.8)

set (SRC_LIST ${PROJECT_SOURCE_DIR}/src/iicCtrl.c)

add_library (iicCtrl_shared SHARED ${SRC_LIST})
add_library (iicCtrl_static STATIC ${SRC_LIST})

set_target_properties (iicCtrl_shared PROPERTIES OUTPUT_NAME "iicCtrl")
set_target_properties (iicCtrl_static PROPERTIES OUTPUT_NAME "iicCtrl")

set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

set(TOOLCHAIN_DIR "/opt/freescale/usr/local/gcc-4.6.2-glibc-2.13-linaro-multilib-2011.12/fsl-linaro-toolchain")
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/bin/arm-fsl-linux-gnueabi-g++)
set(CMAKE_C_COMPILER   ${TOOLCHAIN_DIR}/bin/arm-fsl-linux-gnueabi-gcc)
set(GNU_FLAGS "-O2 -g")
set(CMAKE_CXX_FLAGS "${GNU_FLAGS} ")
set(CMAKE_C_FLAGS "${GNU_FLAGS}  ")
