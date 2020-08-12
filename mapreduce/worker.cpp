//
// Created by William Liu on 2020-08-06.
//

#include "worker.h"

void mr::Worker::ReportFinish() {
  proto::ReportStageFinishMessage msg;
  msg.set_worker_id(worker_id_);
  msg.set_stage_success(stage_success_);
  msg.set_message(stage_msg_);
  msg.set_elapsed_time(elapsed_time_.count());
  google::protobuf::Empty reply;
  grpc::ClientContext ctx;
  grpc::Status status = stub_->ReportStageFinish(&ctx, msg, &reply);
  if (status.ok()) {
    console_->info("ReportFinish [OK]");
  } else {
    console_->warn("ReportFinish failed [%d], error message [%s]",
                   status.error_code(), status.error_message());
  }
}

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

grpc::Status mr::Worker::Map(grpc::ServerContext* context,
                             const mr::proto::MapRequest* request,
                             google::protobuf::Empty* response) {
  auto start = std::chrono::steady_clock::now();
  mapper_->map(request->key(), request->value());
  auto end = std::chrono::steady_clock::now();
  elapsed_time_ = end - start;
  if (error.empty()) {
    stage_success_ = true;
  }
  else {
    stage_success_ = false;
    stage_msg_ = error;
  }
  ReportFinish();
  return grpc::Status::OK;
}

grpc::Status mr::Worker::Reduce(grpc::ServerContext* context,
                                const mr::proto::ReduceRequest* request,
                                google::protobuf::Empty* response) {
  return grpc::Status::OK;
}