#include "controller.hpp"

#include <spdlog/spdlog.h>

#include <filesystem>

namespace aoc {

namespace fs = std::filesystem;

RegisterCommand day02("day02", "day02.txt", [](fs::path filename) -> std::tuple<long, long> {
  spdlog::debug("filename: {}", filename.string());
  return {1, 2};
});

} // namespace aoc