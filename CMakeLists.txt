cmake_minimum_required(VERSION 3.10)
project(MapReduce)

set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_CXX_STANDARD 14)

# cereal
include_directories(${CMAKE_SOURCE_DIR}/3rd_party/cereal/include)

add_subdirectory(${CMAKE_SOURCE_DIR}/mapreduce)
add_subdirectory(${CMAKE_SOURCE_DIR}/demo)