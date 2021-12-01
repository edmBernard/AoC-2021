#include "controller.hpp"

#include "aoc_rust.h"

#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>

namespace aoc {

namespace fs = std::filesystem;

RegisterCommand day01("day01", {
    { "input_day01.txt",       1233,   1275},
    { "input_day01_test1.txt", 7,   5},
  }, [](fs::path filename) -> std::tuple<uint64_t, uint64_t> {

    std::ifstream infile(filename);
    if (!infile.is_open()) {
      throw std::runtime_error(fmt::format("File Not Found : {}", filename.string()));
    }

    std::vector<uint64_t> depthList;
    std::string line;
    while (infile >> line) {
      depthList.push_back(std::stoi(line));
    }

    // part1
    uint64_t part1Result = 0;
    {
      uint64_t previous = depthList[0];
      for (auto depth : depthList) {
        part1Result += depth > previous ? 1 : 0;
        previous = depth;
      }
    }

    // part2
    uint64_t part2Result = 0;
    {
      uint64_t previous = depthList[0] + depthList[1] + depthList[2];
      for (int idx = 0; idx < depthList.size() - 2; ++idx) {
        const int sum = depthList[idx] + depthList[idx+1] + depthList[idx+2];
        part2Result += sum > previous ? 1 : 0;
        previous = sum;
      }
    }
    return {part1Result, part2Result};
});

RegisterCommand day01rust("day01,rust", {
    { "input_day01.txt",       1233,   1275},
    { "input_day01_test1.txt", 7,   5},
  }, [](fs::path filename) -> std::tuple<uint64_t, uint64_t> {
  uint64_t part1 = 0;
  uint64_t part2 = 0;
  rust::day01(filename.string(), part1, part2);
  return {part1, part2};
});

} // namespace aoc