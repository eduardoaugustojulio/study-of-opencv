cmake_minimum_required(VERSION 2.8)
project(OpenCL)

add_compile_options(-std=c99 -pthread -D__GXX_EXPERIMENTAL_CXX0X__ -D__GCC_HAVE_SYNC_COMPARE_AND_SWAP_1 -D__GCC_HAVE_SYNC_COMPARE_AND_SWAP_2 -D__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4 -D__GCC_HAVE_SYNC_COMPARE_AND_SWAP_8)
set(PROJECT_PATH	${CMAKE_CURRENT_SOURCE_DIR})
set(SRCS_PATH		"${PROJECT_PATH}/src")
set(HOME $ENV{HOME})

set(OPENCV_DEFAULT_LIB_PATH "")
if( ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "arm")
    message(STATUS "SET UP OPENCV TO ${CMAKE_SYSTEM_PROCESSOR}")    
    set(OPENCV_DEFAULT_LIB_PATH "${HOME}/opencv3/build-arm/" "${HOME}/opencv/build-arm/")

endif()

find_package(OpenCL REQUIRED)
find_package(OpenCV REQUIRED HINTS ${OPENCV_DEFAULT_LIB_PATH})

message(STATUS ${OpenCL_INCLUDE_DIR})
include_directories(${OPENCL_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS})

file(GLOB SRCS  "${SRCS_PATH}/*.cpp" "${SRCS_PATH}/*.h")

add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${OpenCL_LIBRARY} CLC GAL)
