//
// Created by William Liu on 2020-08-06.
//

#include "mapper.h"

#include <experimental/filesystem>
#include <fstream>
#include <unordered_map>
#include <vector>

namespace fs = std::experimental::filesystem;

void mr::Mapper::write_mapper_output(const std::string& working_dir) {
  // reducer_id -> {value ...}
  std::unordered_map<uint32_t, std::vector<std::string>> temp;

  for (const auto& pair : mapper_output_) {
    // TODO: a user-define-able hashing function?
    // Of type size_t.
    auto hashed_key = std::hash<std::string>{}(pair.first);
    uint32_t reducer_id = hashed_key % num_workers_;
    temp[reducer_id].push_back(pair.second);
  }

  for (const auto& pair : temp) {
    const std::string reducer_path(working_dir + "/" +
                                   std::to_string(pair.first) + "/");
    const fs::path reducer_dir(reducer_path);
    if (!fs::exists(reducer_dir)) fs::create_directory(reducer_dir);
    // <cwd>/<reducer_id>/<mapper_id>.txt
    std::ofstream output(reducer_path + std::to_string(mapper_id_) + ".txt");
    for (const auto& val : pair.second) output << val << std::endl;
  }
}