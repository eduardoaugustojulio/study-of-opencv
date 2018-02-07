cmake_minimum_required(VERSION 2.8)
project(constrast)

add_compile_options(-std=c++11)
SET(PLATFORM_SPECIFIC_LIBS "-lpthread")
set(PROJECT_PATH	${CMAKE_CURRENT_SOURCE_DIR})
set(SRCS_PATH		"${PROJECT_PATH}/src")

set(ZBAR_LIBS 	"zbar")
find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

file(GLOB SRCS  "${SRCS_PATH}/*.cpp" "${SRCS_PATH}/*.h")

add_executable(${PROJECT_NAME}  ${SRCS})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${ZBAR_LIBS} pthread)
