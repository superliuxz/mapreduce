//
// Created by William Liu on 2020-08-04.
//

#ifndef MAPREDUCE_MAPREDUCE_MASTER_H_
#define MAPREDUCE_MAPREDUCE_MASTER_H_

#include <string>
#include <unordered_map>

#include "task.h"
#include "worker_identity.h"

namespace mr {

typedef void (*JoinReduceOutputFunc)();

class Master {
  // TODO:
  // - rpc call that receives the task definition, and create corresponding
  //   mappers and reducers.
 public:
 private:
  // IP -> identity.
  std::unordered_map<std::string, mr::Identity> mappers_;
  // Join the output from reducers. Optional.
  JoinReduceOutputFunc join_reduce_output_fn_;
};
}  // namespace mr

#endif  // MAPREDUCE_MAPREDUCE_MASTER_H_
