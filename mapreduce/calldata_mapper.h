//
// Created by William Liu on 2020-08-14.
//

#ifndef MAPREDUCE_MAPREDUCE_CALLDATA_MAPPER_H_
#define MAPREDUCE_MAPREDUCE_CALLDATA_MAPPER_H_

#include "mapper.h"
#include "worker.grpc.pb.h"
#include "worker.h"
#include "worker.pb.h"

namespace mr {
// Modified from
// https://github.com/grpc/grpc/blob/master/examples/cpp/helloworld/greeter_async_server.cc#L73
//
// Class encompasing the state and logic needed to serve a request.
class CallDataMapper {
 public:
  // Take in the "service" instance (in this case representing an asynchronous
  // server) and the completion queue "cq" used for asynchronous communication
  // with the gRPC runtime.
  CallDataMapper(mr::proto::Worker::AsyncService* service,
                 grpc::ServerCompletionQueue* cq)
      : service_(service), cq_(cq), responder_(&ctx_) {}
  void MapAndDeleteSelf(mr::MapperBase* mapper, const std::string& work_dir);

 private:
  // The means of communication with the gRPC runtime for an asynchronous
  // server.
  mr::proto::Worker::AsyncService* service_;
  // The producer-consumer queue where for asynchronous server notifications.
  grpc::ServerCompletionQueue* cq_;
  // Context for the rpc, allowing to tweak aspects of it such as the use
  // of compression, authentication, as well as to send metadata back to the
  // client.
  grpc::ServerContext ctx_;
  // What we get from the client.
  mr::proto::MapRequest request_;
  // What we send back to the client.
  mr::proto::MapReply reply_;
  // The means to get back to the client.
  grpc::ServerAsyncResponseWriter<mr::proto::MapReply> responder_;
};
}  // namespace mr

#endif  // MAPREDUCE_MAPREDUCE_CALLDATA_MAPPER_H_
