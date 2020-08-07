include_directories(${CMAKE_SOURCE_DIR}/mapreduce)

add_executable(demo demo.cpp)
target_link_libraries(demo MapReduce)