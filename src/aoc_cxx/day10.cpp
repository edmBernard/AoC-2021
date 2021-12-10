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
#include <variant>

namespace aoc {

namespace fs = std::filesystem;
namespace rs = ranges;
namespace rv = ranges::views;

namespace {

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

uint64_t getScorePart1(char c) {
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

uint64_t getScorePart2(char c) {
  switch (c) {
    case ')':
      return 1;
    case ']':
      return 2;
    case '}':
      return 3;
    case '>':
      return 4;
    default:
      throw std::runtime_error("Unkown symbol : " + c);
  }
}

std::variant<char, uint64_t> checkLine(const std::string& line) {
  std::vector<char> stack;

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

  uint64_t count = 0;
  for (int i = stack.size() - 1; i >= 0; --i) {
    count *= 5;
    count += getScorePart2(getComplement(stack[i]));
  }
  return count;
}


} // namespace


RegisterCommand day10("day10", {
    { "input_day10.txt",       243939, 2421222841},
    { "input_day10_test1.txt", 26397,  288957},
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
    std::vector<uint64_t> scoreListPart2;

    for (auto& line : puzzleInput) {
      auto result = checkLine(line);
      if (std::holds_alternative<char>(result)) {
        countPart1 += getScorePart1(std::get<char>(result));
      } else {
        scoreListPart2.push_back(std::get<1>(result));
      }

    }

    std::sort(scoreListPart2.begin(), scoreListPart2.end());

    uint64_t countPart2 = scoreListPart2[scoreListPart2.size() / 2];

    return {countPart1, countPart2};
});


} // namespace aoc
