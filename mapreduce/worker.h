//
// Created by William Liu on 2020-08-06.
//

#ifndef MAPREDUCE_MAPREDUCE_WORKER_H_
#define MAPREDUCE_MAPREDUCE_WORKER_H_

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include <cstdint>
#include <memory>
#include <string>

#include "mapper.h"
#include "reducer.h"
#include "task.h"
#include "worker.pb.h"
#include "worker_identity.h"

namespace mr {
class Worker {
 public:
  explicit Worker(uint32_t wid)
      : identity_(mr::Identity::mapper), worker_id_(wid) {}
  ~Worker() = default;

  // TODO: RPC calls
  void set_worker_id(uint32_t worker_id);  // RPC call from Master
  void set_task(const mr::Task& task);     // RPC call from Master
  void map();                              // RPC call from Master
  void reduce();                           // RPC call from Master

 private:
  mr::Identity identity_;
  uint32_t worker_id_;
  mr::Task task_;
  std::unique_ptr<mr::Mapper> mapper_;
  std::unique_ptr<mr::Reducer> reducer_;
  // Used to merge the result from reducer, optional.
  mr::MergeReduceResultFn merge_func_ = nullptr;
};
}  // namespace mr

#endif  // MAPREDUCE_MAPREDUCE_WORKER_H_
