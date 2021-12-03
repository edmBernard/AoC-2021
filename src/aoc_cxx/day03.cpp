#include "controller.hpp"

#include "aoc_rust.h"

#include <range/v3/all.hpp>
#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>
#include <vector>
#include <tuple>
#include <charconv>

namespace aoc {

namespace fs = std::filesystem;
namespace rs = ranges;
namespace rv = ranges::views;


// std::vector<uint64_t> histogram(const std::vector<uint16_t>& inputPuzzle, uint64_t bit, uint64_t lineLength) {
//   std::vector<uint64_t> occurenceOfBit(lineLength, 0);
//   for (size_t i = 0; i < lineLength; ++i) {
//     const uint64_t cursor = 1 << i;
//     if (
//     occurenceOfBit[occurenceOfBit.size()-i-1] +=
//     for (auto& number : inputPuzzle) {
//       if (line[i] == '0') {
//         occurenceOf0[i] += 1;
//       } else {
//         occurenceOf1[i] += 1;
//       }
//     }
//   }
//   return occurenceOfBit;
// }

RegisterCommand day03("day03", {
    { "input_day03.txt",       3429254,   5388270},
    { "input_day03_test1.txt", 198,       230},
  }, [](fs::path filename) -> std::tuple<uint64_t, uint64_t> {

    std::ifstream infile(filename);
    if (!infile.is_open()) {
      throw std::runtime_error(fmt::format("File Not Found : {}", filename.string()));
    }

    std::vector<uint16_t> inputPuzzle;
    std::vector<uint64_t> occurenceOf0;
    std::vector<uint64_t> occurenceOf1;
    std::string line;
    size_t lineLength = 0;
    while (getline(infile, line)) {

      uint16_t result;
      const auto [ptr, ec] = std::from_chars(line.data(), line.data() + line.size(), result, 2);
      if (ec != std::errc())
        throw std::runtime_error(fmt::format("Fail to parse : {}", line));

      inputPuzzle.push_back(result);

      if (lineLength == 0) {
        lineLength = line.size();
        occurenceOf0 = std::vector<uint64_t>(lineLength, 0);
        occurenceOf1 = std::vector<uint64_t>(lineLength, 0);
      }

      for (size_t i = 0; i < line.size(); ++i) {
        if (line[i] == '0') {
          occurenceOf0[i] += 1;
        } else {
          occurenceOf1[i] += 1;
        }
      }
    }

    uint64_t gammaRate = 0;
    uint64_t epsilonRate = 0;
    for (size_t i = 0, shift = lineLength -1; i < lineLength; ++i, --shift) {
      gammaRate += (occurenceOf0[i] > occurenceOf1[i] ? 0 : 1) << shift;
      epsilonRate += (occurenceOf0[i] > occurenceOf1[i] ? 1 : 0) << shift;
    }
    const uint64_t part1Result = gammaRate * epsilonRate;


    uint64_t oxygen = 0;
    uint64_t co2 = 0;

    std::sort(inputPuzzle.begin(), inputPuzzle.end());
    for (auto& v : inputPuzzle) {
      spdlog::debug("line={:#07b}", v);
    }

    {
      auto begin = inputPuzzle.begin();
      auto end = inputPuzzle.end();
      for (size_t i = 0, shift = lineLength -1; i < lineLength; ++i, --shift) {
        auto pos = std::find_if(begin, end, [&](auto value) { return (value >> shift) & 1; });
        if (pos != end) {
          spdlog::debug("first={:#07b} last={:#07b} size={} pos={:#07b} ({}) iteration={}", *begin, *(end-1), end - begin, *pos, *pos, i);
          if (pos - begin < end - pos) {
            begin = pos;
          } else {
            end = pos;
          }
          oxygen = *pos;
        }
      }
    }

    {
      auto begin = inputPuzzle.begin();
      auto end = inputPuzzle.end();
      for (size_t i = 0, shift = lineLength -1; i < lineLength; ++i, --shift) {
        auto pos = std::find_if(begin, end, [&](auto value) { return (value >> shift) & 1; });
        if (pos != end) {
          spdlog::debug("first={:#07b} last={:#07b} size={} pos={:#07b} ({}) iteration={}", *begin, *(end-1), end - begin, *pos, *pos, i);
          if (pos - begin <= end - pos) {
            end = pos;
          } else {
            begin = pos;
          }
          co2 = *pos;
        }
      }
    }

    const uint64_t part2Result = oxygen * co2;

    return {part1Result, part2Result};
});

} // namespace aoc