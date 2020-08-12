add_executable(demo demo.cpp)
target_link_libraries(demo MapReduce)
target_include_directories(demo PUBLIC ${CMAKE_SOURCE_DIR}/mapreduce)