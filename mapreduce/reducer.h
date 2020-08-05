//
// Created by William Liu on 2020-08-04.
//
#ifndef MAPREDUCE__REDUCER_H_
#define MAPREDUCE__REDUCER_H_

#include <string>

#include "task.h"

namespace mr {
class Reducer {
  explicit Reducer(const mr::Task& task) {}
  // TODO: what's the type of value???
  virtual void reduce(const std::string& key, void* value) = 0;
};
}  // namespace mr

#endif  // MAPREDUCE__REDUCER_H_
