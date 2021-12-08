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
    { "input_day08.txt",       495, 1055164},
    { "input_day08_test1.txt", 26,  61229},
  }, [](fs::path filename) -> std::tuple<uint64_t, uint64_t> {

    std::ifstream infile(filename);
    if (!infile.is_open()) {
      throw std::runtime_error(fmt::format("File Not Found : {}", filename.string()));
    }

    std::string line;
    std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> puzzleInput;


    while (getline(infile, line)) {
      std::vector<std::string> inputLine = split(line, '|');
      std::vector<std::string> wire = split(inputLine[0], ' ');
      std::vector<std::string> outputSignal = split(inputLine[1], ' ');
      puzzleInput.push_back({wire, outputSignal});
    }

    // part1
    uint64_t countPart1 = 0;
    for (auto& [wire, outputSignal] : puzzleInput) {
      for (auto& s : outputSignal) {
        if (s.size() == 2 || s.size() == 4 || s.size() == 3 || s.size() == 7)
          countPart1 += 1;
      }
    }

    // part2
    uint64_t countPart2 = 0;
    // with unique number of segment
    // known 1 4 7 8

    // 6 segments and contain all segment from 4 -> 9
    // 6 segments remaining and contain all segment from 7  -> 0
    // 6 segments remaining  -> 6
    // known + 0 6 9

    // 5 segments and contain all segment from 7 -> 3
    // 5 segments remaining and contain exactly 2 segment from 4  -> 2
    // 5 segments remaining  -> 5
    // known + 3 2 5
    auto is1 = [](std::string s) { return s.size() == 2; };
    auto is4 = [](std::string s) { return s.size() == 4; };
    auto is7 = [](std::string s) { return s.size() == 3; };
    auto is8 = [](std::string s) { return s.size() == 7; };

    for (auto& [wire, outputSignal] : puzzleInput) {
      std::unordered_map<std::string, uint8_t> segmentToValue;
      std::vector<std::string> number6segment;
      std::vector<std::string> number5segment;
      std::string seven;
      std::string four;

      for (auto& w : wire) {
        std::sort(w.begin(), w.end());
        if (is1(w)) segmentToValue[w] = 1;
        if (is4(w)) {
          segmentToValue[w] = 4;
          four = w;
        }
        if (is7(w)) {
          segmentToValue[w] = 7;
          seven = w;
        }
        if (is8(w)) segmentToValue[w] = 8;
        if (w.size() == 6) number6segment.push_back(w);
        if (w.size() == 5) number5segment.push_back(w);
      }

      // find 6 segment numbers
      // find 9
      int indexOf9 = 15;
      for (int i = 0; i < number6segment.size(); ++i) {
        auto pos = number6segment[i].find(four[0]);
        if (pos == std::string::npos)
          continue;
        pos = number6segment[i].find(four[1]);
        if (pos == std::string::npos)
          continue;
        pos = number6segment[i].find(four[2]);
        if (pos == std::string::npos)
          continue;
        pos = number6segment[i].find(four[3]);
        if (pos == std::string::npos)
          continue;
        indexOf9 = i;
        segmentToValue[number6segment[i]] = 9;
      }
      number6segment.erase(number6segment.begin() + indexOf9);

      // find 0
      int indexOf0 = 15;
      for (int i = 0; i < number6segment.size(); ++i) {
        auto pos = number6segment[i].find(seven[0]);
        if (pos == std::string::npos)
          continue;
        pos = number6segment[i].find(seven[1]);
        if (pos == std::string::npos)
          continue;
        pos = number6segment[i].find(seven[2]);
        if (pos == std::string::npos)
          continue;
        indexOf0 = i;
        segmentToValue[number6segment[i]] = 0;
      }
      number6segment.erase(number6segment.begin() + indexOf0);

      // find 6
      segmentToValue[number6segment[0]] = 6;

      // find 5 segment numbers
      // find 3
      int indexOf3 = 15;
      for (int i = 0; i < number5segment.size(); ++i) {
        auto pos = number5segment[i].find(seven[0]);
        if (pos == std::string::npos)
          continue;
        pos = number5segment[i].find(seven[1]);
        if (pos == std::string::npos)
          continue;
        pos = number5segment[i].find(seven[2]);
        if (pos == std::string::npos)
          continue;
        indexOf3 = i;
        segmentToValue[number5segment[i]] = 3;
      }
      number5segment.erase(number5segment.begin() + indexOf3);

      // find 2
      int indexOf2 = 15;
      for (int i = 0; i < number5segment.size(); ++i) {
        int count = 0;
        auto pos = number5segment[i].find(four[0]);
        if (pos != std::string::npos)
          ++count;
        pos = number5segment[i].find(four[1]);
        if (pos != std::string::npos)
          ++count;
        pos = number5segment[i].find(four[2]);
        if (pos != std::string::npos)
          ++count;
        pos = number5segment[i].find(four[3]);
        if (pos != std::string::npos)
          ++count;
        if (count == 2) {
          indexOf2 = i;
          segmentToValue[number5segment[i]] = 2;
        }
      }
      number5segment.erase(number5segment.begin() + indexOf2);

      // find 5
      segmentToValue[number5segment[0]] = 5;

      // compute resulting value
      int value = 0;
      for (auto& s : outputSignal) {
        std::sort(s.begin(), s.end());
        value = value * 10 + segmentToValue[s];
      }
      countPart2 += value;
    }

    return {countPart1, countPart2};
});


} // namespace aoc
