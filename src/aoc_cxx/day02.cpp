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
    { "input_day02.txt",       2117664,   2073416724},
    { "input_day02_test1.txt", 150,       900},
  }, [](fs::path filename) -> std::tuple<uint64_t, uint64_t> {

    std::ifstream infile(filename);
    if (!infile.is_open()) {
      throw std::runtime_error(fmt::format("File Not Found : {}", filename.string()));
    }

    std::vector<std::pair<Direction, uint16_t>> inputPuzzle;
    std::string line;
    while (getline(infile, line)) {

      const auto match = line.find(" ");
      if (match == std::string::npos)
        throw std::runtime_error(fmt::format("Fail to parse : {}", line));

      uint16_t result;
      // +1 to escape matching space to remains in value
      const auto [ptr, ec] = std::from_chars(line.data() + match + 1, line.data() + line.size(), result);
      if (ec != std::errc())
        throw std::runtime_error(fmt::format("Fail to parse : {}", line));

      std::string_view direction(line.data(), match);
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

    uint64_t position = 0;
    uint64_t depthPart1 = 0;
    uint64_t depthPart2 = 0;
    uint64_t aim = 0;
    for (auto& [dir, value] : inputPuzzle) {
      switch (dir)
      {
      case Direction::Forward:
        position += value;
        depthPart2 += value * aim;
        break;
      case Direction::Up:
        depthPart1 -= value;
        aim -= value;
        break;
      case Direction::Down:
        depthPart1 += value;
        aim += value;
        break;
      }
    }
    uint64_t part1Result = depthPart1 * position;
    uint64_t part2Result = depthPart2 * position;

    return {part1Result, part2Result};
});

RegisterRustCommand day02rustfunctional("day02,rust,functional", {
    { "input_day02.txt",       2117664,   2073416724},
    { "input_day02_test1.txt", 150,       900},
  }, rust::day02functional);

} // namespace aoc