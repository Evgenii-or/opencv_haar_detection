cmake_minimum_required(VERSION 3.10)

project(detector)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(lib_sources result.cpp template.cpp cascade.cpp)

set(lib_headers result.h template.h cascade.h)

add_library(detector STATIC ${lib_headers} ${lib_sources})

install(FILES $<TARGET_FILE:opencv_world> DESTINATION $<TARGET_FILE_DIR:${PROJECT_NAME}>)


