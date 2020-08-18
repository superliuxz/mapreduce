//
// Created by William Liu on 2020-08-14.
//

#ifndef MAPREDUCE_MAPREDUCE_CALLDATA_REDUCER_H_
#define MAPREDUCE_MAPREDUCE_CALLDATA_REDUCER_H_

#include "reducer.h"
#include "worker.grpc.pb.h"
#include "worker.pb.h"

namespace mr {
class CallDataReducer {
 public:
  CallDataReducer(mr::proto::Worker::AsyncService* service,
                  grpc::ServerCompletionQueue* cq)
      : service_(service), cq_(cq), responder_(&ctx_) {}
  void ReduceAndDeleteSelf(mr::Reducer* reducer, const std::string& work_dir);

 private:
  mr::proto::Worker::AsyncService* service_;
  grpc::ServerCompletionQueue* cq_;
  grpc::ServerContext ctx_;
  mr::proto::ReduceRequest request_;
  mr::proto::ReduceReply reply_;
  grpc::ServerAsyncResponseWriter<mr::proto::ReduceReply> responder_;
};
}  // namespace mr

#endif  // MAPREDUCE_MAPREDUCE_CALLDATA_REDUCER_H_
