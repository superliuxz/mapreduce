//
// Created by William Liu on 2020-08-04.
//
#ifndef MAPREDUCE__REDUCER_H_
#define MAPREDUCE__REDUCER_H_

#include <string>
#include <vector>

namespace mr {
class Reducer {
 public:
  Reducer() = default;
  explicit Reducer(uint32_t reducer_id, uint32_t num_workers)
      : reducer_id_(reducer_id), num_workers_(num_workers) {}
  virtual ~Reducer() = default;

  // User-defined functions.
  virtual void reduce(const std::string& key, const std::string& value,
                      std::string& error) {}
  // Read input for reduce stage. The default implementation should be good
  // enough.
  virtual void read_mapper_output(const std::string& working_dir);

 private:
  uint32_t reducer_id_{};
  uint32_t num_workers_{};
  std::vector<std::string> mapper_output_;
};
}  // namespace mr

#endif  // MAPREDUCE__REDUCER_H_
