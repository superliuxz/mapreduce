//
// Created by William Liu on 2020-08-04.
//

#ifndef MAPREDUCE_MAPREDUCE_MASTER_H_
#define MAPREDUCE_MAPREDUCE_MASTER_H_

#include <string>
#include <unordered_map>

#include "task.h"

namespace mr {
enum class Identity { mapper, reducer };
class Master {
  // TODO:
  // - rpc call that receives the task definition, and create corresponding
  //   mappers and reducers.
 public:
 private:
  std::unordered_map<std::string, mr::Identity> mappers_;
};
}  // namespace mr

#endif  // MAPREDUCE_MAPREDUCE_MASTER_H_
