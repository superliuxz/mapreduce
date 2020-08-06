//
// Created by William Liu on 2020-08-06.
//

#ifndef MAPREDUCE_MAPREDUCE_WORKER_H_
#define MAPREDUCE_MAPREDUCE_WORKER_H_

#include <cstdint>
#include <string>

#include "mapper.h"
#include "reducer.h"
#include "worker_identity.h"

namespace mr {
class worker {
 public:
  uint32_t worker_id;
  mr::Identity identity;

 private:
  mr::Mapper mapper_;
  mr::Reducer reducer_;
  // Used to merge the result from reducer, optional.
  mr::MergeReduceResultFn merge_func_;
};
}  // namespace mr

#endif  // MAPREDUCE_MAPREDUCE_WORKER_H_
