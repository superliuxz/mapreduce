//
// Created by William Liu on 2020-08-04.
//
#ifndef MAPREDUCE__MAPPER_H_
#define MAPREDUCE__MAPPER_H_

#include <string>

#include "task.h"

namespace mr {
class Mapper {
  explicit Mapper(const mr::Task& task) {}
  virtual void map(const std::string& key, const std::string& value) = 0;
};
}  // namespace mr

#endif  // MAPREDUCE__MAPPER_H_
