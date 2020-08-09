//
// Created by William Liu on 2020-08-04.
//

#ifndef MAPREDUCE_MAPREDUCE_MASTER_H_
#define MAPREDUCE_MAPREDUCE_MASTER_H_

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include <string>
#include <unordered_map>

#include "master.pb.h"
#include "task.h"
#include "worker_identity.h"

namespace mr {

typedef void (*JoinReduceOutputFunc)();

// using mr_proto::MasterStub;

class Master {
  // TODO:
  // - rpc call that receives the task definition, and create corresponding
  //   mappers and reducers.
 public:
 private:
  // IP -> identity.
  std::unordered_map<std::string, mr::Identity> workers_;
  // Join the output from reducers. Optional.
  JoinReduceOutputFunc join_reduce_output_fn_ = nullptr;
};
}  // namespace mr

#endif  // MAPREDUCE_MAPREDUCE_MASTER_H_
