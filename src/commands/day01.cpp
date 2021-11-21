#include "controller.hpp"

#include <spdlog/spdlog.h>

#include <filesystem>

namespace aoc {

namespace fs = std::filesystem;

RegisterCommand day01("day01", "day01.txt", [](fs::path filename) -> std::tuple<long, long> {
  spdlog::debug("filename: {}", filename.string());
  return {1, 2};
});

RegisterCommand day01Part1Test("day01,part1,test", "day01_part1.txt", [](fs::path filename) -> std::tuple<long, long> {
  spdlog::debug("filename: {}", filename.string());
  return {1, 2};
});

} // namespace aoc