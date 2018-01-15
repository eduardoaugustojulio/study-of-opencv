cmake_minimum_required(VERSION 2.8)
project(OpenCL)

add_compile_options(-std=c++11)
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

include_directories(${OPENCL_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS})

file(GLOB SRCS  "${SRCS_PATH}/*.cpp" "${SRCS_PATH}/*.h")

add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${OPENCL_LIBRARIES} CLC GAL)
