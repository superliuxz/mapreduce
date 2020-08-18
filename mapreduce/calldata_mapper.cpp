//
// Created by William Liu on 2020-08-18.
//

#include "calldata_mapper.h"

void mr::CallDataMapper::MapAndDeleteSelf(mr::Mapper* mapper, const std::string& work_dir) {
  // We *request* that the system start processing Map requests. In this
  // request, "this" acts are the tag uniquely identifying the request (so
  // that different CallData instances can serve different requests
  // concurrently), in this case the memory address of this CallData
  // instance.
  service_->RequestMap(&ctx_, &request_, &responder_, cq_, cq_, this);

  std::string error;
  auto start = std::chrono::steady_clock::now();
  mapper->map(request_.key(), request_.value(), error);
  auto end = std::chrono::steady_clock::now();
  reply_.set_message(error);
  reply_.set_stage_success(error.empty());
  reply_.set_elapsed_time((end - start).count());
  mapper->write_mapper_output(work_dir);

  // And we are done! Let the gRPC runtime know we've finished, using the
  // memory address of this instance as the uniquely identifying tag for
  // the event.
  responder_.Finish(reply_, grpc::Status::OK, this);
  delete this;
}