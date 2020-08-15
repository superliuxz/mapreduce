//
// Created by William Liu on 2020-08-04.
//

#ifndef MAPREDUCE_MAPREDUCE_TASK_H_
#define MAPREDUCE_MAPREDUCE_TASK_H_

#include "mapper.h"
#include "reducer.h"

namespace mr {



struct Task {
  uint32_t num_workers;
  std::vector<std::string> workers;
  std::string work_directory{"/tmp/map_reduce/"};
  std::string input_name;

  mr::Mapper mapper;
  std::vector<std::string> mapper_keys;
  std::vector<std::string> mapper_values;
  mr::Reducer reducer;
  std::vector<std::string> reducer_keys;
  std::vector<std::string> reducer_values;
};

}  // namespace mr

#endif  // MAPREDUCE_MAPREDUCE_TASK_H_
