#include "controller.hpp"

#include "aoc_rust.h"

#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>
#include <charconv>

namespace aoc {

namespace fs = std::filesystem;

enum Direction {
  Up,
  Down,
  Forward
};

RegisterCommand day02("day02", {
    { "input_day02.txt",       2117664,   1275},
    { "input_day02_test1.txt", 150,   5},
  }, [](fs::path filename) -> std::tuple<uint64_t, uint64_t> {

    std::ifstream infile(filename);
    if (!infile.is_open()) {
      throw std::runtime_error(fmt::format("File Not Found : {}", filename.string()));
    }

    std::vector<std::pair<Direction, uint16_t>> inputPuzzle;
    std::string line;
    while (getline(infile, line)) {

      const auto match = line.find(" ");
      if (match == std::string::npos) {
        throw std::runtime_error(fmt::format("Fail to parse : {}", line));
      }

      std::string_view direction(line.data(), match);
      std::string_view value(line.data() + match);

      uint16_t result;
      // +1 to escape matching space to remains in value
      const auto [ptr, ec] = std::from_chars(line.data() + match + 1, line.data() + line.size(), result);
      if (ec != std::errc())
        throw std::runtime_error(fmt::format("Fail to parse : {}", line));

      if (direction == "forward") {
        inputPuzzle.emplace_back(Direction::Forward, result);
      } else if (direction == "up") {
        inputPuzzle.emplace_back(Direction::Up, result);
      } else if (direction == "down") {
        inputPuzzle.emplace_back(Direction::Down, result);
      } else {
        throw std::runtime_error(fmt::format("Unrecognize direction : {}", direction));
      }
    }

    // part1
    uint64_t position = 0;
    uint64_t depth = 0;
    for (auto& [dir, value] : inputPuzzle) {
      switch (dir)
      {
      case Direction::Forward:
        position += value;
        break;
      case Direction::Up:
        depth -= value;
        break;
      case Direction::Down:
        depth += value;
        break;
      }
    }
    spdlog::debug("depth={} position={}", depth, position);
    uint64_t part1Result = depth * position;

    // part2
    uint16_t part2Result = 0;
    // uint16_t previous = depthList[0] + depthList[1] + depthList[2];
    // for (int idx = 2; idx < depthList.size(); ++idx) {
    //   const uint16_t sum = depthList[idx] + depthList[idx-1] + depthList[idx-2];
    //   if (sum > previous) {
    //     ++part2Result;
    //   }
    //   previous = sum;
    // }
    return {part1Result, part2Result};
});

RegisterCommand day02rust("day02,rust", {{"day02.txt", 1, 2}}, [](fs::path filename) -> std::tuple<uint64_t, uint64_t> {
  uint64_t part1;
  uint64_t part2;
  rust::day02(filename.string(), part1, part2);
  spdlog::debug("result from rust: part1 {} part2 {}", part1, part2);
  return {part1, part2};
});

} // namespace aoc