#include "controller.hpp"

#include "aoc_rust.h"

namespace aoc {

namespace fs = std::filesystem;

RegisterRustCommand day13rust("day13,rust", {
    { "input_day13.txt",       716,    97},
    { "input_day13_test1.txt", 17,     16},
  }, rust::day13);

} // namespace aoc
