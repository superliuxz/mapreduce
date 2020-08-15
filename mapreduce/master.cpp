//
// Created by William Liu on 2020-08-04.
//

#include "master.h"

#include <worker.pb.h>

// TODO: error handling.
void mr::Master::run() {
  map_();
  reduce_();
}

void mr::Master::map_() {
  grpc::CompletionQueue cq;
  std::vector<mr::proto::MapReply> replies(task_.workers.size());
  std::vector<grpc::Status> st(task_.workers.size());
  std::vector<uint64_t> tags(task_.workers.size());

  for (auto i = 0; i < task_.workers.size(); ++i) {
    tags[i] = i;
    mr::proto::MapRequest request;
    request.set_key(task_.mapper_keys[i]);
    request.set_value(task_.mapper_values[i]);
    grpc::ClientContext ctx;

    const std::string& ip = task_.workers[i];
    auto channel = grpc::CreateChannel(ip, grpc::InsecureChannelCredentials());
    std::unique_ptr<mr::proto::Worker::Stub> stub(
        mr::proto::Worker::NewStub(channel));

    // stub_->PrepareAsyncMap() creates an RPC object, returning an
    // instance to store in "call" but does not actually start the RPC
    // Because we are using the asynchronous API, we need to hold on to
    // the "call" instance in order to get updates on the ongoing RPC.
    std::unique_ptr<grpc::ClientAsyncResponseReader<mr::proto::MapReply> > rpc(
        stub->PrepareAsyncMap(&ctx, request, &cq));
    // StartCall initiates the RPC call
    rpc->StartCall();
    // Request that, upon completion of the RPC, "reply" be updated with
    // the server's response; "status" with the indication of whether the
    // operation was successful. Tag the request with the address of tags[i].
    rpc->Finish(&replies[i], &st[i], (void*)&tags[i]);
  }
  uint64_t finished = 0;
  while (finished++ < task_.workers.size()) {
    void* got_tag;
    bool ok = false;
    // Block until the next result is available in the completion queue
    // "cq". The return value of Next should always be checked. This
    // return value tells us whether there is any kind of event or the cq_
    // is shutting down.
    GPR_ASSERT(cq.Next(&got_tag, &ok));
    // Verify that the result from "cq" corresponds, by its tag, our
    // previous request.
    auto id = static_cast<int*>(got_tag);
    // ... and that the request was completed successfully. Note that "ok"
    // corresponds solely to the request for updates introduced by
    // Finish().
    GPR_ASSERT(ok);

    if (replies[*id].stage_success())
      console_->info("Worker [%s] finished Map within [%d] seconds",
                     task_.workers[*id], replies[*id].elapsed_time());
    else
      console_->info("Worker [%s] FAILED Map within [%d] seconds. ERR: %s",
                     task_.workers[*id], replies[*id].elapsed_time(),
                     replies[*id].message());
  }
  cq.Shutdown();
}

void mr::Master::reduce_() {
  grpc::CompletionQueue cq;
  std::vector<mr::proto::ReduceReply> replies(task_.workers.size());
  std::vector<grpc::Status> st(task_.workers.size());
  std::vector<uint64_t> tags(task_.workers.size());

  for (auto i = 0; i < task_.workers.size(); ++i) {
    tags[i] = i;
    mr::proto::ReduceRequest request;
    request.set_key(task_.mapper_keys[i]);
    request.set_value(task_.mapper_values[i]);
    grpc::ClientContext ctx;

    const std::string& ip = task_.workers[i];
    auto channel = grpc::CreateChannel(ip, grpc::InsecureChannelCredentials());
    std::unique_ptr<mr::proto::Worker::Stub> stub(
        mr::proto::Worker::NewStub(channel));

    std::unique_ptr<grpc::ClientAsyncResponseReader<mr::proto::ReduceReply> >
        rpc(stub->PrepareAsyncReduce(&ctx, request, &cq));

    rpc->StartCall();

    rpc->Finish(&replies[i], &st[i], (void*)&tags[i]);
  }
  uint64_t finished = 0;
  while (finished++ < task_.workers.size()) {
    void* got_tag;
    bool ok = false;

    GPR_ASSERT(cq.Next(&got_tag, &ok));

    auto id = static_cast<int*>(got_tag);

    GPR_ASSERT(ok);

    if (replies[*id].stage_success())
      console_->info("Worker [%s] finished Reduce within [%d] seconds",
                     task_.workers[*id], replies[*id].elapsed_time());
    else
      console_->info("Worker [%s] FAILED Reduce within [%d] seconds. ERR: %s",
                     task_.workers[*id], replies[*id].elapsed_time(),
                     replies[*id].message());
  }
  cq.Shutdown();
  if (join_reduce_output_fn_ != nullptr) join_reduce_output_fn_();
}