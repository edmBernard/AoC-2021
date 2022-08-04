#include "controller.hpp"
#include "utils.hpp"

#include "aoc_rust.h"

#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>
#include <vector>
#include <tuple>
#include <charconv>

namespace aoc {

namespace fs = std::filesystem;

namespace {
uint64_t treeDescent(const std::vector<uint16_t> &inputPuzzle, size_t lineLength, bool option) {
  auto begin = inputPuzzle.begin();
  auto end = inputPuzzle.end();
  // We reduce the search at each iteration by moving begin and end of the search
  for (size_t i = 0, shift = lineLength - 1; i < lineLength; ++i, --shift) {
    auto pos = std::find_if(begin, end, [&](auto value) { return (value >> shift) & 1; });
    if (pos != end) {
      if ((pos - begin <= end - pos) ^ option) {
        begin = pos;
      } else {
        end = pos;
      }
    }
    if (end - begin == 1)
      break;
  }
  return *begin;
}
} // namespace

RegisterCommand day03("day03", {
    { "input_day03.txt",       3429254,   5410338},
    { "input_day03_test1.txt", 198,       230},
  }, [](fs::path filename) -> std::array<uint64_t, 2> {

    std::ifstream infile(filename);
    if (!infile.is_open()) {
      throw std::runtime_error(fmt::format("File Not Found : {}", filename.string()));
    }

    std::vector<uint16_t> inputPuzzle;
    std::vector<uint64_t> occurenceOf0;
    std::string line;
    size_t lineLength = 0;
    while (getline(infile, line)) {

      inputPuzzle.push_back(aoc::parse<uint16_t>(line, 2));

      if (lineLength == 0) {
        lineLength = line.size();
        occurenceOf0 = std::vector<uint64_t>(lineLength, 0);
      }

      for (size_t i = 0; i < line.size(); ++i) {
        if (line[i] == '0') {
          occurenceOf0[i] += 1;
        }
      }
    }

    // part1
    uint64_t gammaRate = 0;
    for (size_t i = 0, shift = lineLength -1; i < lineLength; ++i, --shift) {
      gammaRate += occurenceOf0[i] >= (inputPuzzle.size() - occurenceOf0[i]) ? 0 : (1 << shift);
    }
    const uint64_t epsilonRate = (1 << lineLength) - 1 & ~gammaRate;

    const uint64_t part1Result = gammaRate * epsilonRate;

    // part2
    // We sort to create a tree-like with 0 and 1
    std::sort(inputPuzzle.begin(), inputPuzzle.end());

    const uint64_t oxygen = treeDescent(inputPuzzle, lineLength, true);
    const uint64_t co2 = treeDescent(inputPuzzle, lineLength, false);

    const uint64_t part2Result = oxygen * co2;

    return {part1Result, part2Result};
});


RegisterRustCommand day03rustfunctional("day03,rust,functional", {
    { "input_day03.txt",       3429254,   5410338},
    { "input_day03_test1.txt", 198,       230},
  }, rust::day03functional);

} // namespace aoc