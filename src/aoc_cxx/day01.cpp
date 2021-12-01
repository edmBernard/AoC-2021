#include "controller.hpp"

#include "aoc_rust.h"

#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>
#include <charconv>

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

    std::vector<uint16_t> depthList;
    std::string line;
    // getline(infile, line) is faster than infile >> line
    // from_char is faster than stol
    // istream_iterator<uint16_t> is also slower than this while loop
    while (getline(infile, line)) {
      uint16_t result;
      const auto [ptr, ec] = std::from_chars(line.data(), line.data() + line.size(), result);
      if (ec != std::errc())
        throw std::runtime_error(fmt::format("Fail to parse : {}", line));
      depthList.push_back(result);
    }

    // part1
    uint16_t part1Result = 0;
    for (int idx = 1; idx < depthList.size(); ++idx) {
      part1Result += depthList[idx] > depthList[idx-1] ? 1 : 0;
    }

    // part2
    uint16_t part2Result = 0;
    uint16_t previous = depthList[0] + depthList[1] + depthList[2];
    for (int idx = 3; idx < depthList.size(); ++idx) {
      const uint16_t sum = depthList[idx] + depthList[idx-1] + depthList[idx-2];
      part2Result += sum > previous ? 1 : 0;
      previous = sum;
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

RegisterCommand day01rustv2("day01,rust,v2", {
    { "input_day01.txt",       1233,   1275},
    { "input_day01_test1.txt", 7,   5},
  }, [](fs::path filename) -> std::tuple<uint64_t, uint64_t> {
  uint64_t part1 = 0;
  uint64_t part2 = 0;
  rust::day01(filename.string(), part1, part2);
  return {part1, part2};
});

} // namespace aoc