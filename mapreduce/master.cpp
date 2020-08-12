//
// Created by William Liu on 2020-08-04.
//

#include "master.h"

grpc::Status mr::Master::ReportStageFinish(
    grpc::ServerContext *context,
    const proto::ReportStageFinishMessage *request,
    google::protobuf::Empty *response) {
  if (request->stage_success()) {
    console_->info(
        "Worker [%d](%s) ReportStageFinish SUCCESS with message [%s]; "
        "elapsed_time [%d].",
        request->worker_id(), workers_[request->worker_id()].first,
        request->message(), request->elapsed_time());
    workers_[request->worker_id()].second =
        workers_[request->worker_id()].second == mr::Identity::mapper
            ? mr::Identity::reducer
            : mr::Identity::mapper;
  } else {
    console_->warn(
        "Worker [%d](%s) ReportStageFinish FAILED with message [%s]; "
        "elapsed_time [%d].",
        request->worker_id(), workers_[request->worker_id()].first,
        request->message(), request->elapsed_time());
  }
  return grpc::Status::OK;
}