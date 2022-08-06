#include "controller.hpp"

#include "aoc_rust.h"

namespace aoc {

namespace fs = std::filesystem;

const RegisterRustCommand day15rust("day15,rust", {
    { "input_day15.txt",       441,  2188189693529},
    { "input_day15_test1.txt",  40,  2188189693529},
  }, rust::day15);

} // namespace aoc
