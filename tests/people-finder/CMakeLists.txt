cmake_minimum_required(VERSION 2.8)
project(find-people)

add_compile_options(-std=c++11)
set(PROJECT_PATH	${CMAKE_CURRENT_SOURCE_DIR})
set(SRCS_PATH		"${PROJECT_PATH}/src")

find_package(OpenCV REQUIRED)

file(GLOB SRCS  "${SRCS_PATH}/*.cpp" "${SRCS_PATH}/*.h")

add_executable(${PROJECT_NAME}  ${SRCS})

target_link_libraries(${PROJECT_NAME}  ${OpenCV_LIBS})
