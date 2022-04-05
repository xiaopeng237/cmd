include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(. SRC_LIST)

add_executable(cmd ${SRC_LIST})

TARGET_LINK_LIBRARIES(cmd dl)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

set(TOOLCHAIN_DIR "/opt/freescale/usr/local/gcc-4.6.2-glibc-2.13-linaro-multilib-2011.12/fsl-linaro-toolchain")
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/bin/arm-fsl-linux-gnueabi-g++)
set(CMAKE_C_COMPILER   ${TOOLCHAIN_DIR}/bin/arm-fsl-linux-gnueabi-gcc)
set(GNU_FLAGS "-O2 -g")
set(CMAKE_CXX_FLAGS "${GNU_FLAGS} ")
set(CMAKE_C_FLAGS "${GNU_FLAGS}  ")

