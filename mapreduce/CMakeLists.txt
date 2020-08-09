add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/proto)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(MapReduce SHARED mapper.cpp master.cpp reducer.cpp worker.cpp)
target_link_libraries(MapReduce
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})

# add included directory for the generated proto/grpc *.pb.h
target_include_directories(MapReduce PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/proto)

# add explicit dependencies to generate proto headers and srcs.
add_dependencies(MapReduce mr-proto)