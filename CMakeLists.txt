cmake_minimum_required(VERSION 3.10)
project(MapReduce)

set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_CXX_STANDARD 14)

add_library(MapReduce SHARED library.cpp library.h
    mapreduce/mapper.h mapreduce/reducer.h mapreduce/master.cpp
    mapreduce/master.h mapreduce/task.h mapreduce/worker.h mapreduce/worker_identity.h mapreduce/mapper.cpp mapreduce/reducer.cpp)