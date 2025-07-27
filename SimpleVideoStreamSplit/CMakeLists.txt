cmake_minimum_required(VERSION 3.16)

project(SimpleVideoStreamSplit LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_executable(split_video main.cpp)

target_include_directories(split_video PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(split_video PRIVATE ${OpenCV_LIBS})
