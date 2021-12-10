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
#include <optional>

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


template <typename T>
void showBoard(const std::vector<T> &mark, std::tuple<uint64_t, uint64_t> size) {
  const auto [width, height] = size;
  for (size_t row = 0; row < height; ++row) {
    for (size_t col = 0; col < width; ++col) {
      fmt::print("{} ", mark[row * width + col]);
    }
    fmt::print("\n");
  }
}

char getComplement(char c) {
  switch (c) {
    case '(':
      return ')';;
    case ')':
      return '(';;
    case '[':
      return ']';
    case ']':
      return '[';
    case '{':
      return '}';
    case '}':
      return '{';
    case '<':
      return '>';
    case '>':
      return '<';
    default:
      throw std::runtime_error("Unkown symbol : " + c);
  }
}

uint64_t getScore(char c) {
  switch (c) {
    case ')':
      return 3;
    case ']':
      return 57;
    case '}':
      return 1197;
    case '>':
      return 25137;
    default:
      throw std::runtime_error("Unkown symbol : " + c);
  }
}

std::optional<char> checkLine(const std::string& line) {
  std::vector<char> stack;
  int countRound = 0;  // round bracket
  int countSquare = 0;  // square bracket
  int countCurly = 0;  // curly bracket
  int countAngle = 0;  // angle bracket

  for (char c : line) {
    switch (c) {
      case '(':
        stack.push_back(c);
        break;
      case ')':
        if (c != getComplement(stack.back())) {
          return c;
        }
        stack.pop_back();
        break;
      case '[':
        stack.push_back(c);
        break;
      case ']':
        if (c != getComplement(stack.back())) {
          return c;
        }
        stack.pop_back();
        break;
      case '{':
        stack.push_back(c);
        break;
      case '}':
        if (c != getComplement(stack.back())) {
          return c;
        }
        stack.pop_back();
        break;
      case '<':
        stack.push_back(c);
        break;
      case '>':
        if (c != getComplement(stack.back())) {
          return c;
        }
        stack.pop_back();
        break;
      default:
        throw std::runtime_error("Unkown symbol : " + c);
    }
  }
  return {};
}


} // namespace


RegisterCommand day10("day10", {
    { "input_day10.txt",       243939, 1038240},
    { "input_day10_test1.txt", 26397,  1134},
  }, [](fs::path filename) -> std::tuple<uint64_t, uint64_t> {

    std::ifstream infile(filename);
    if (!infile.is_open()) {
      throw std::runtime_error(fmt::format("File Not Found : {}", filename.string()));
    }

    std::string line;
    std::vector<std::string> puzzleInput;

    while (getline(infile, line)) {
      puzzleInput.push_back(line);
    }

    uint64_t countPart1 = 0;

    for (auto& line : puzzleInput) {
      auto result = checkLine(line);
      if (result) {
        countPart1 += getScore(result.value());
      }

    }

    uint64_t countPart2 = 0;

    return {countPart1, countPart2};
});


} // namespace aoc
