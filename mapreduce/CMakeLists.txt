include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(MapReduce SHARED mapper.cpp master.cpp reducer.cpp)