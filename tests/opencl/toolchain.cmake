cmake_minimum_required(VERSION 2.8)

#set target system
set(CMAKE_SYSTEM_NAME	Linux)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR arm)

#specify the cross compiler
set(CMAKE_C_COMPILER /opt/poky/1.6.2/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-gcc)
set(CMAKE_CXX_COMPILER /opt/poky/1.6.2/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-g++)

set(CMAKE_C_FLAGS   "-march=armv7-a -mcpu=cortex-a9 -mtune=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard"  ${CMAKE_C_FLAGS})
set(CMAKE_CXX_FLAGS "-march=armv7-a -mcpu=cortex-a9 -mtune=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard"  ${CMAKE_CXX_FLAGS})

#set where is the targer environment 
set(CMAKE_SYSROOT /opt/poky/1.6.2/sysroots/cortexa9hf-vfp-neon-poky-linux-gnueabi)

# search for programs in the build host environment 
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# search for libraries and headers in the target environment 
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
