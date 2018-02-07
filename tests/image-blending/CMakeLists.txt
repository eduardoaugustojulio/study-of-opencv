cmake_minimum_required(VERSION 2.8)
project(image-blending)

add_compile_options(-std=c++11)
SET(PLATFORM_SPECIFIC_LIBS "-lpthread")
set(PROJECT_PATH	${CMAKE_CURRENT_SOURCE_DIR})
set(SRCS_PATH		"${PROJECT_PATH}/src")

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

file(GLOB SRCS  "${SRCS_PATH}/*.cpp" "${SRCS_PATH}/*.h")

add_executable(${PROJECT_NAME}  ${SRCS})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} pthread)
