#include "controller.hpp"

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
    fmt::print("'{}' ", mark[col]);
  }
  fmt::print("\n");
}


} // namespace


RegisterCommand day08("day08", {
    { "input_day08.txt",       495,  92948968},
    { "input_day08_test1.txt", 26,      61229},
  }, [](fs::path filename) -> std::tuple<uint64_t, uint64_t> {

    std::ifstream infile(filename);
    if (!infile.is_open()) {
      throw std::runtime_error(fmt::format("File Not Found : {}", filename.string()));
    }

    std::string line;
    std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> puzzleInput;

    uint64_t countPart1 = 0;

    while (getline(infile, line)) {
      std::vector<std::string> inputLine = split(line, '|');
      std::vector<std::string> wire = split(inputLine[1], ' ');
      std::vector<std::string> outputSignal = split(inputLine[1], ' ');
      puzzleInput.push_back({wire, outputSignal});
    }

    // showVect(outputSignal);
    for (auto& [wire, outputSignal] : puzzleInput) {
      for (auto& s : outputSignal) {
        // fmt::print("{}\n", s);
        if (s.size() == 2 || s.size() == 4 || s.size() == 3 || s.size() == 7)
          countPart1 += 1;
      }
    }

    uint64_t countPart2 = 0;
    return {countPart1, countPart2};
});


} // namespace aoc
