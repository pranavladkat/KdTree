project(kdtree)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
file (GLOB ${SRC_LIST}
     "*.hpp"
     "*.cpp")
SET(CMAKE_CXX_FLAGS "-std=c++11")
