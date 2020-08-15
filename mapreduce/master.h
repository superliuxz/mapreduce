//
// Created by William Liu on 2020-08-04.
//

#ifndef MAPREDUCE_MAPREDUCE_MASTER_H_
#define MAPREDUCE_MAPREDUCE_MASTER_H_

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <worker.grpc.pb.h>

#include <string>
#include <unordered_map>

#include "master.grpc.pb.h"
#include "master.pb.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include "task.h"
#include "worker_identity.h"

namespace mr {

typedef void (*JoinReduceOutputFunc)();
typedef void (*MergeReduceResultFn)();

class Master {
 public:
  void run();

 private:
  mr::Task task_;
  // Join the output from reducers. Optional.
  JoinReduceOutputFunc join_reduce_output_fn_ = nullptr;
  // logger
  std::shared_ptr<spdlog::logger> console_ = spdlog::stdout_color_mt("console");
  // Master makes async RPC call to workers.
  void map_();
  void reduce_();
};
}  // namespace mr

#endif  // MAPREDUCE_MAPREDUCE_MASTER_H_
