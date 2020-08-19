//
// Created by William Liu on 2020-08-19.
//

#include "map_reduce.h"

#include <string>

#include "cstdlib"
#include "master.h"
#include "worker.h"

void mr::MapReduce::Run(const mr::Task& task, mr::MapperBase&& mapper,
                        mr::ReducerBase&& reducer) {
  std::string ip(std::getenv("MR_IP"));
  if (task.master == ip) {
    // Master
    mr::Master master(task);
    master.run();
  } else {
    // Worker
    mr::Worker worker(task, std::move(mapper), std::move(reducer));
    // TODO:
    //  worker should send out heart beat to let master know that it's ready.
    worker.run();
  }
}