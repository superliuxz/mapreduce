//
// Created by William Liu on 2020-08-04.
//

#ifndef MAPREDUCE_MAPREDUCE_TASK_H_
#define MAPREDUCE_MAPREDUCE_TASK_H_

#include "mapper.h"
#include "reducer.h"

namespace mr {

typedef void (*MergeReduceResultFn)();

struct Task {
  uint32_t num_workers;
  std::vector<std::string> workers;
  std::string work_directory{"/tmp/map_reduce/"};
  std::string input_name;

  mr::Mapper mapper;
  mr::Reducer reducer;
  MergeReduceResultFn merge_func = nullptr;
};

}  // namespace mr

#endif  // MAPREDUCE_MAPREDUCE_TASK_H_
