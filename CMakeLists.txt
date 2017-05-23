project(leetcode)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

set (EXTRA_CXXFLAGS "-std=c++11")

# add extra flags to CFLAGS and CXXFLAGS
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CXXFLAGS}")
