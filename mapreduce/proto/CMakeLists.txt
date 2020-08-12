# reference:
# https://stackoverflow.com/questions/60408160/how-to-compile-proto-file-using-cmake-to-generate-grpc-pb-cc-and-grpc-pb-h-fi
# https://stackoverflow.com/questions/32823563/using-cmake-to-create-protobuf-grpc-cc-files
# https://github.com/grpc/grpc/blob/master/examples/cpp/helloworld/CMakeLists.txt


function(GEN_PROTO_GRPC SRCS HDRS)

  set(${SRCS})
  set(${HDRS})
  foreach(FIL ${ARGN})
    get_filename_component(ABS_FIL ${FIL} ABSOLUTE) # abs proto path
    get_filename_component(FIL_WE ${FIL} NAME_WE) # filename without ext
    get_filename_component(proto_path "${ABS_FIL}" PATH) # directory with all protos

    set(proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.cc")
    set(proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.h")
    set(grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.grpc.pb.cc")
    set(grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.grpc.pb.h")

    list(APPEND ${SRCS} ${proto_srcs})
    list(APPEND ${SRCS} ${grpc_srcs})
    list(APPEND ${HDRS} ${proto_hdrs})
    list(APPEND ${HDRS} ${grpc_hdrs})

    add_custom_command(
        OUTPUT "${proto_srcs}" "${proto_hdrs}" "${grpc_srcs}" "${grpc_hdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
             --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
             --proto_path "${proto_path}"
             --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${ABS_FIL}"
        DEPENDS "${ABS_FIL}")
  endforeach()

  set(${SRCS} ${${SRCS}} PARENT_SCOPE)
  set(${HDRS} ${${HDRS}} PARENT_SCOPE)
endfunction()

file(GLOB proto_files "${CMAKE_CURRENT_SOURCE_DIR}/*.proto")
GEN_PROTO_GRPC(PROTO_SRCS PROTO_HDRS ${proto_files})
# pass PROTO_SRCS to parent scope so we can add PROTO_SRCS to the lib.
set(PROTO_SRCS ${PROTO_SRCS} PARENT_SCOPE)

message(STATUS "To be generated proto srcs and hdrs:
${PROTO_SRCS}
${PROTO_HDRS}")
add_custom_target(mr-proto ALL DEPENDS ${PROTO_SRCS} ${PROTO_HDRS})