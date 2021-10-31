include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(. SRC_LIST)

add_executable(test ${SRC_LIST})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

set(TOOLCHAIN_DIR "/home/vmuser/arm-fsl-linux-gnueabi")
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/bin/arm-fsl-linux-gnueabi-g++)
set(CMAKE_C_COMPILER   ${TOOLCHAIN_DIR}/bin/arm-fsl-linux-gnueabi-gcc)
set(GNU_FLAGS "-O2 -g")
set(CMAKE_CXX_FLAGS "${GNU_FLAGS} ")
set(CMAKE_C_FLAGS "${GNU_FLAGS}  ")