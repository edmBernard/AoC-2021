#include "controller.hpp"
#include "utils.hpp"

#include "aoc_rust.h"

#include <range/v3/all.hpp>
#include <spdlog/spdlog.h>

#include <charconv>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <map>
#include <array>
#include <algorithm>
#include <numeric>

namespace aoc {

namespace fs = std::filesystem;
namespace rs = ranges;
namespace rv = ranges::views;

namespace {

inline std::vector<std::string> split(const std::string &original, char separator) {
  std::vector<std::string> results;
  auto start = original.begin();
  auto end = original.end();
  auto next = std::find(start, end, separator);

  while (next != end) {
    results.push_back(std::string(start, next));
    start = next + 1;
    next = std::find(start, end, separator);
  }

  results.push_back(std::string(start, next));

  return results;
}


template <typename T>
inline std::vector<T> parse(const std::vector<std::string> &input, int base = 10) {
  std::vector<T> result(input.size());
  for (size_t i = 0; i < input.size(); ++i) {
    const auto [ptr, ec] = std::from_chars(input[i].data(), input[i].data() + input[i].size(), result[i], base);
    if (ec != std::errc())
      throw std::runtime_error(fmt::format("Fail to parse : {}", input[i]));
  }
  return result;
}


template <typename T>
void showVect(const std::vector<T> &mark) {
  for (size_t col = 0; col < mark.size(); ++col) {
    fmt::print("{} ", mark[col]);
  }
  fmt::print("\n");
}

template <typename T>
uint64_t computePart1(const std::vector<T>& input, T position) {
  uint64_t sum = 0;
  for (auto& p : input) {
    sum += std::abs(p - position);
  }
  return sum;
}
template <typename T>
uint64_t computePart2(const std::vector<T>& input, T position) {
  uint64_t sum = 0;
  for (auto& p : input) {
    sum += (std::abs(p - position) + 1) * std::abs(p - position) / 2;
  }
  return sum;
}

} // namespace


RegisterCommand day07("day07", {
    { "input_day07.txt",       340052,  92948968},
    { "input_day07_test1.txt", 37,      168},
  }, [](fs::path filename) -> std::tuple<uint64_t, uint64_t> {

    std::ifstream infile(filename);
    if (!infile.is_open()) {
      throw std::runtime_error(fmt::format("File Not Found : {}", filename.string()));
    }

    std::string line;
    getline(infile, line);
    std::vector<int32_t> puzzleInput = aoc::splitString(line, ',');

    auto [pmin, pmax] = std::minmax_element(puzzleInput.begin(), puzzleInput.end());

    // part1
    // dichotomy method to reduce the search space
    uint64_t fuelPart1 = 0;
    {
      std::pair<int, uint64_t> left{*pmin, computePart1(puzzleInput, *pmin)};
      std::pair<int, uint64_t> right{*pmax, computePart1(puzzleInput, *pmax)};

      while (right.first - left.first > 1) {
        int pos = (right.first + left.first) / 2;
        uint64_t sum = computePart1(puzzleInput, pos);
        if (left.second < right.second) {
          right = {pos, sum};
        } else {
          left = {pos, sum};
        }
      }
      fuelPart1 = std::min(right.second, left.second);
    }

    // part2
    // dichotomy method to reduce the search space
    uint64_t fuelPart2 = 0;
    {
      std::pair<int, uint64_t> left{*pmin, computePart2(puzzleInput, *pmin)};
      std::pair<int, uint64_t> right{*pmax, computePart2(puzzleInput, *pmax)};

      while (right.first - left.first > 1) {
        int pos = (right.first + left.first) / 2;
        uint64_t sum = computePart2(puzzleInput, pos);
        if (left.second < right.second) {
          right = {pos, sum};
        } else {
          left = {pos, sum};
        }
      }
      fuelPart2 = std::min(right.second, left.second);
    }
    return {fuelPart1, fuelPart2};
});


} // namespace aoc
