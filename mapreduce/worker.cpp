//
// Created by William Liu on 2020-08-06.
//

#include "worker.h"

#include "calldata_mapper.h"
#include "calldata_reducer.h"

void mr::Worker::HandleMapRPC() {
  new CallDataMapper(&service_, cq_.get());
  void* tag;  // uniquely identifies a request.
  bool ok;
  // Block waiting to read the next event from the completion queue. The
  // event is uniquely identified by its tag, which in this case is the
  // memory address of a CallData instance.
  // The return value of Next should always be checked. This return value
  // tells us whether there is any kind of event or cq_ is shutting down.
  GPR_ASSERT(cq_->Next(&tag, &ok));
  GPR_ASSERT(ok);
  static_cast<CallDataMapper*>(tag)->MapAndDeleteSelf(mapper_.get(),
                                                      task_.work_directory);
}

void mr::Worker::HandleReduceRPC() {
  new CallDataReducer(&service_, cq_.get());
  void* tag;
  bool ok;
  GPR_ASSERT(cq_->Next(&tag, &ok));
  GPR_ASSERT(ok);
  static_cast<CallDataReducer*>(tag)->ReduceAndDeleteSelf(reducer_.get(),
                                                          task_.work_directory);
}