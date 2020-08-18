//
// Created by William Liu on 2020-08-18.
//

#include "calldata_reducer.h"

void mr::CallDataReducer::ReduceAndDeleteSelf(mr::Reducer *reducer,
                                              const std::string &work_dir) {
  service_->RequestReduce(&ctx_, &request_, &responder_, cq_, cq_, this);

  reducer->read_mapper_output(work_dir);
  std::string error;
  auto start = std::chrono::steady_clock::now();
  // value is read from the mapper's output
  reducer->reduce(request_.key(), request_.value(), error);
  auto end = std::chrono::steady_clock::now();
  reply_.set_message(error);
  reply_.set_stage_success(error.empty());
  reply_.set_elapsed_time((end - start).count());

  responder_.Finish(reply_, grpc::Status::OK, this);
  delete this;
}