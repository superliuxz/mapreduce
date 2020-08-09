//
// Created by William Liu on 2020-08-04.
//
#ifndef MAPREDUCE__MAPPER_H_
#define MAPREDUCE__MAPPER_H_

#include <set>
#include <string>

namespace mr {
class Mapper {
 public:
  Mapper() = default;
  Mapper(uint32_t mapper_id, uint32_t num_workers)
      : mapper_id_(mapper_id), num_workers_(num_workers) {}
  virtual ~Mapper() = default;

  // User-defined functions.

  // Split the input based on worker's id.
  virtual void split_input(const std::string& input_loc) {}
  // Map function that does the work.
  virtual void map(const std::string& key, const std::string& value) {}

  // Flush the output from map stage onto storage. The default implementation
  // is good enough.
  virtual void write_mapper_output(const std::string& working_dir);

 private:
  uint32_t mapper_id_{};
  uint32_t num_workers_{};
  // A multiset of key-val pair. The "key" will be used to calculate which
  // reducer the "value" will be sent to. Each value corresponds to each
  // record/line from the splitted input.
  std::multiset<std::pair<std::string, std::string>> mapper_output_;
};
}  // namespace mr

#endif  // MAPREDUCE__MAPPER_H_
