//
// Created by William Liu on 2020-08-06.
//

#include "reducer.h"

#include <experimental/filesystem>
#include <fstream>

namespace fs = std::experimental::filesystem;

void mr::Reducer::read_mapper_output(const std::string& working_dir) {
  // <cwd>/<reducer_id>/
  const std::string reducer_path(working_dir + "/" +
                                 std::to_string(reducer_id_) + "/");
  const fs::path reducer_dir(reducer_path);
  if (!fs::exists(reducer_dir)) return;

  for (const auto& file : fs::directory_iterator(reducer_dir)) {
    if (file.path().extension().generic_string() == ".txt") {
      std::string value;
      std::ifstream ifs(file.path().string());
      while (std::getline(ifs, value)) {
        mapper_output_.push_back(value);
      }
    }
  }
}