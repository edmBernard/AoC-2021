#include "controller.hpp"

#include "aoc_rust.h"

#include <spdlog/spdlog.h>

#include <filesystem>

namespace aoc {

namespace fs = std::filesystem;

RegisterCommand day02("day02", {{"day02.txt", 1, 2}}, [](fs::path filename) -> std::tuple<size_t, size_t> {
  size_t part1;
  size_t part2;
  rust::day02(filename.string(), part1, part2);
  spdlog::debug("result from rust: part1 {} part2 {}", part1, part2);
  return {part1, part2};
});

} // namespace aoc