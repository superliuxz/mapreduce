//
// Created by William Liu on 2020-08-19.
//

#ifndef MAPREDUCE_MAPREDUCE_MAP_REDUCE_H_
#define MAPREDUCE_MAPREDUCE_MAP_REDUCE_H_

#include "task.h"

namespace mr {
class MapReduce {
  static void Run(const mr::Task& task, mr::MapperBase&& mapper,
                  mr::ReducerBase&& reducer);
};
}  // namespace mr

#endif  // MAPREDUCE_MAPREDUCE_MAP_REDUCE_H_
