cmake_minimum_required(VERSION 3.10)

project(app LANGUAGES CXX)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories("../detector")

set(prj_sources main.cpp)

add_executable(${PROJECT_NAME} ${prj_sources})
target_link_libraries(${PROJECT_NAME} detector ${OpenCV_LIBS})

set_property(TARGET app PROPERTY CXX_STANDARD 17)
install(FILES $<TARGET_FILE:opencv_world> DESTINATION $<TARGET_FILE_DIR:${PROJECT_NAME}>)

