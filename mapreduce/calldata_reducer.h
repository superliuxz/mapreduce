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
                  grpc::ServerCompletionQueue* cq, mr::Reducer* reducer,
                  const std::string& work_dir)
      : service_(service), cq_(cq), responder_(&ctx_) {
    service_->RequestReduce(&ctx_, &request_, &responder_, cq_, cq_, this);

    reducer->read_mapper_output(work_dir);
    std::string error;
    auto start = std::chrono::steady_clock::now();
    // value is read from the mapper's output
    reducer->reduce(request_.key(), nullptr, error);
    auto end = std::chrono::steady_clock::now();
    reply_.set_message(error);
    reply_.set_stage_success(error.empty());
    reply_.set_elapsed_time((end - start).count());

    responder_.Finish(reply_, grpc::Status::OK, this);
    delete this;
  }

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
