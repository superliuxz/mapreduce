//
// Created by William Liu on 2020-08-06.
//

#include "worker.h"

grpc::Status mr::Worker::SetWorkerId(
    grpc::ServerContext* context, const mr::proto::SetWorkerIdRequest* request,
    google::protobuf::Empty* response) {
  if (worker_id_ != -1) {
    console_->warn("Master is resetting worker [%d] to [%d]", worker_id_,
                   request->worker_id());
  }
  worker_id_ = request->worker_id();
  return grpc::Status::OK;
}

grpc::Status mr::Worker::SetTask(grpc::ServerContext* context,
                                 const mr::proto::SetTaskRequest* request,
                                 google::protobuf::Empty* response) {
  return grpc::Status::OK;
}
