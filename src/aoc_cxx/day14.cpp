#include "controller.hpp"

#include "aoc_rust.h"

namespace aoc {

namespace fs = std::filesystem;

RegisterRustCommand day14rust("day14,rust", {
    { "input_day14.txt",       3587,  2188189693529},
    { "input_day14_test1.txt", 1588,  2188189693529},
  }, rust::day14);

} // namespace aoc
