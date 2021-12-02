#include "controller.hpp"

#include "aoc_rust.h"

#include <range/v3/all.hpp>
#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>
#include <charconv>

namespace aoc {

namespace fs = std::filesystem;
namespace rs = ranges;
namespace rv = ranges::views;


enum Direction {
  Up,
  Down,
  Forward
};





std::pair<Direction, uint16_t> parseInputLine(std::string_view direction, std::string_view value) {

  uint16_t result;
  const auto [ptr, ec] = std::from_chars(value.data(), value.data() + value.size(), result);
  if (ec != std::errc())
    throw std::runtime_error(fmt::format("Fail to parse : {}", value));

  if (direction == "forward") {
    return {Direction::Forward, result};
  } else if (direction == "up") {
    return {Direction::Up, result};
  } else if (direction == "down") {
    return {Direction::Down, result};
  }
  throw std::runtime_error(fmt::format("Unrecognize direction : '{}'", direction));

  return {};
}


std::pair<Direction, uint16_t> parseInputLine(std::string_view line) {

  const auto match = line.find(" ");
  if (match == std::string::npos)
    throw std::runtime_error(fmt::format("Can't find space in : {}", line));

  // +1 to escape matching space to remains in value
  return parseInputLine({line.data(), match}, line.data() + match + 1);
}


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
      inputPuzzle.push_back(parseInputLine(line));
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
    const uint64_t part1Result = depthPart1 * position;
    const uint64_t part2Result = depthPart2 * position;

    return {part1Result, part2Result};
});


RegisterCommand day02range("day02,range", {
    { "input_day02.txt",       2117664,   2073416724},
    { "input_day02_test1.txt", 150,       900},
  }, [](fs::path filename) -> std::tuple<uint64_t, uint64_t> {

    std::ifstream infile(filename);
    if (!infile.is_open()) {
      throw std::runtime_error(fmt::format("File Not Found : {}", filename.string()));
    }

    // istream_view split by space and new line. the output is an interleaved vector with direction and value
    const std::vector<std::pair<Direction, uint16_t>> inputPuzzle = rs::istream_view<std::string>(infile)
        | rv::chunk(2)
        | rv::transform([](auto line) {
            std::vector<std::string> asVector = line | rs::to<std::vector>;
            return parseInputLine(asVector[0], asVector[1]); })
        | rs::to<std::vector>;

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
    const uint64_t part1Result = depthPart1 * position;
    const uint64_t part2Result = depthPart2 * position;

    return {part1Result, part2Result};
});


RegisterRustCommand day02rustfunctional("day02,rust,functional", {
    { "input_day02.txt",       2117664,   2073416724},
    { "input_day02_test1.txt", 150,       900},
  }, rust::day02functional);

} // namespace aoc