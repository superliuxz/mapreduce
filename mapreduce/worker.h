//
// Created by William Liu on 2020-08-06.
//

#ifndef MAPREDUCE_MAPREDUCE_WORKER_H_
#define MAPREDUCE_MAPREDUCE_WORKER_H_

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include <chrono>
#include <cstdint>
#include <memory>
#include <string>

#include "mapper.h"
#include "master.grpc.pb.h"
#include "master.pb.h"
#include "reducer.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include "task.h"
#include "worker.grpc.pb.h"
#include "worker.pb.h"
#include "worker_identity.h"

namespace mr {
class Worker final : public mr::proto::Worker::Service {
 public:
  explicit Worker(const std::shared_ptr<grpc::Channel>& channel)
      : identity_(mr::Identity::mapper),
        stub_(proto::Master::NewStub(channel)) {}
  ~Worker() override = default;

  // Make the RPC call to Master, reporting the finish of current stage finish.
  void ReportFinish();

  // Worker as the server receives RPC calls from Master.
  grpc::Status SetWorkerId(grpc::ServerContext* context,
                           const mr::proto::SetWorkerIdRequest* request,
                           google::protobuf::Empty* response) override;
  grpc::Status SetTask(grpc::ServerContext* context,
                       const mr::proto::SetTaskRequest* request,
                       google::protobuf::Empty* response) override;
  // TODO: make Map and Reduce async, so no need for ReportFinish call.
  grpc::Status Map(grpc::ServerContext* context,
                   const mr::proto::MapRequest* request,
                   google::protobuf::Empty* response) override;
  grpc::Status Reduce(grpc::ServerContext* context,
                      const mr::proto::ReduceRequest* request,
                      google::protobuf::Empty* response) override;
  // TODO: two other RPC call, SetToMapper and SetToReducer.
  //   both calls reset timer, identity, stage_success_ and stage_msg_.
 private:
  // Current identity: mapper or reducer.
  mr::Identity identity_;
  // Id of the worker, set by Master.
  uint32_t worker_id_ = -1;
  // The task definition.
  mr::Task task_;
  // Two classes that implement the map and reduce logic.
  std::unique_ptr<mr::Mapper> mapper_;
  std::unique_ptr<mr::Reducer> reducer_;
  // The status of finished map/reduce stage.
  bool stage_success_{};
  std::string stage_msg_;
  // Used to merge the result from reducer, optional, set from |task_|.
  mr::MergeReduceResultFn merge_func_ = nullptr;
  // RPC.
  std::unique_ptr<proto::Master::Stub> stub_;
  // Time for the current stage.
  std::chrono::duration<double> elapsed_time_{};
  // logger
  std::shared_ptr<spdlog::logger> console_ = spdlog::stdout_color_mt("console");
};
}  // namespace mr

#endif  // MAPREDUCE_MAPREDUCE_WORKER_H_
