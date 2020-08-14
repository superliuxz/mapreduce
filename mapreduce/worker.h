//
// Created by William Liu on 2020-08-06.
//

#ifndef MAPREDUCE_MAPREDUCE_WORKER_H_
#define MAPREDUCE_MAPREDUCE_WORKER_H_

#include <grpc/support/log.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include <chrono>
#include <cstdint>
#include <memory>
#include <string>

#include "mapper.h"
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
  Worker() = default;
  ~Worker() override {
    server_->Shutdown();
    // Always shutdown the completion queue after the server.
    cq_->Shutdown();
  };

  // Worker as the server receives RPC calls from Master.
  grpc::Status SetWorkerId(grpc::ServerContext* context,
                           const mr::proto::SetWorkerIdRequest* request,
                           google::protobuf::Empty* response) override;
  grpc::Status SetTask(grpc::ServerContext* context,
                       const mr::proto::SetTaskRequest* request,
                       google::protobuf::Empty* response) override;
 private:
  // Id of the worker, set by Master.
  uint32_t worker_id_ = -1;
  // The task definition.
  mr::Task task_;
  // Two classes that implement the map and reduce logic.
  std::unique_ptr<mr::Mapper> mapper_;
  std::unique_ptr<mr::Reducer> reducer_;
  // Used to merge the result from reducer, optional, set from |task_|.
  mr::MergeReduceResultFn merge_func_ = nullptr;

  // logger
  std::shared_ptr<spdlog::logger> console_ = spdlog::stdout_color_mt("console");
  // gRPC async related
  std::unique_ptr<grpc::ServerCompletionQueue> cq_;
  mr::proto::Worker::AsyncService service_;
  std::unique_ptr<grpc::Server> server_;

  // Modified from
  // https://github.com/grpc/grpc/blob/master/examples/cpp/helloworld/greeter_async_server.cc#L73
  //
  // Class encompasing the state and logic needed to serve a request.


};
}  // namespace mr

#endif  // MAPREDUCE_MAPREDUCE_WORKER_H_
