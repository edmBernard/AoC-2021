#include "controller.hpp"

#include <spdlog/spdlog.h>

#include <filesystem>

namespace aoc {

namespace fs = std::filesystem;

RegisterCommand day01("day01", {
    { "day01.txt",        1,  2},
    { "day01_part1.txt",  1,  2},
  }, [](fs::path filename) -> std::tuple<size_t, size_t> {
    spdlog::debug("filename: {}", filename.string());
    return {1, 2};
});

} // namespace aoc