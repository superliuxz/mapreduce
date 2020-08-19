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
#include <utility>

#include "mapper.h"
#include "reducer.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include "task.h"
#include "worker.grpc.pb.h"
#include "worker.pb.h"
#include "worker_identity.h"

namespace mr {
class Worker {
 public:
  explicit Worker(mr::Task task, mr::MapperBase&& mapper,
                  mr::ReducerBase&& reducer)
      : task_(std::move(task)) {
    mapper_ = std::make_unique<mr::MapperBase>(std::move(mapper));
    reducer_ = std::make_unique<mr::ReducerBase>(std::move(reducer));
  }
  ~Worker() {
    server_->Shutdown();
    // Always shutdown the completion queue after the server.
    cq_->Shutdown();
  };
  void HandleMapRPC();
  void HandleReduceRPC();

 private:
  // The task definition.
  mr::Task task_;
  // Two classes that implement the map and reduce logic.
  std::unique_ptr<mr::MapperBase> mapper_;
  std::unique_ptr<mr::ReducerBase> reducer_;

  // logger
  std::shared_ptr<spdlog::logger> console_ = spdlog::stdout_color_mt("console");
  // gRPC async related
  std::unique_ptr<grpc::ServerCompletionQueue> cq_;
  mr::proto::Worker::AsyncService service_;
  std::unique_ptr<grpc::Server> server_;
};
}  // namespace mr

#endif  // MAPREDUCE_MAPREDUCE_WORKER_H_
