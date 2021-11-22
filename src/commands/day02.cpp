#include "controller.hpp"

#include "rust_part.h"

#include <spdlog/spdlog.h>

#include <filesystem>

namespace aoc {

namespace fs = std::filesystem;

RegisterCommand day02("day02", {{"day02.txt", 1, 2}}, [](fs::path filename) -> std::tuple<size_t, size_t> {
  uint16_t result = addition(12, 13);

  spdlog::debug("result from rust: {}", result);
  return {1, 2};
});

} // namespace aoc