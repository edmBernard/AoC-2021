#include "controller.hpp"
#include "utils.hpp"

#include <spdlog/spdlog.h>

#include <charconv>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <tuple>
#include <vector>
#include <array>
#include <algorithm>
#include <numeric>

namespace aoc {

namespace fs = std::filesystem;

namespace {


template <int Dim, typename T>
void showBoard(const std::vector<T> &mark) {
  for (size_t row = 0; row < Dim; ++row) {
    for (size_t col = 0; col < Dim; ++col) {
      fmt::print("{} ", mark[row * Dim + col]);
    }
    fmt::print("\n");
  }
}


template <int Dim, typename T>
void showVect(const std::array<T, Dim> &mark) {
  for (size_t col = 0; col < Dim; ++col) {
    fmt::print("{} ", mark[col]);
  }
  fmt::print("\n");
}

// function that increment the board and increment the counter if needed
template<uint8_t Mask, uint8_t Unity>
void increment(uint8_t& elem, uint64_t& count) {
  switch (elem & Mask) {
    case Unity:
      ++count;
    case 0b0000'0000:
      elem += Unity;
      break;
    default:
      break;
  }
}

} // namespace


RegisterCommand day05("day05", {
    { "input_day05.txt",       7380,  21373},
    { "input_day05_test1.txt", 5,     12},
  }, [](fs::path filename) -> std::array<uint64_t, 2> {

    std::ifstream infile(filename);
    if (!infile.is_open()) {
      throw std::runtime_error(fmt::format("File Not Found : {}", filename.string()));
    }

    std::string line;
    std::vector<std::array<uint16_t, 4>> inputPuzzle;

    while (getline(infile, line)) {

      std::vector<std::string_view> splitted = aoc::splitString(line, ' ');
      std::vector<uint16_t> left = aoc::splitString(splitted[0], ',');
      std::vector<uint16_t> right = aoc::splitString(splitted[2], ',');

      inputPuzzle.push_back({left[0], left[1], right[0], right[1]});

    }

    constexpr uint16_t dim = 1000;
    // We are currently limited in speed by the allocation of these/this boards
    // unfortunately std::map are even slower
    // we need 3 states by part (0, 1, 2+) so we encode these states for both part in 8bits
    // X X X X Y Y Y Y
    //| part2 | part1 |
    constexpr uint8_t maskPart1 = 0b0000'1111;
    constexpr uint8_t unityPart1 = 0b0000'0001;
    constexpr uint8_t maskPart2 = 0b1111'0000;
    constexpr uint8_t unityPart2 = 0b0001'0000;

    std::vector<uint8_t> board(dim*dim, 0);
    uint64_t countPart1 = 0;
    uint64_t countPart2 = 0;

    for (auto& [x1, y1, x2, y2] : inputPuzzle) {
      if (x1 == x2) {
        // vertical line
        for (size_t y = std::min(y1, y2); y <= std::max(y1, y2); ++y) {
          auto& elem = board[y * dim + x1];
          increment<maskPart1, unityPart1>(elem, countPart1);
          increment<maskPart2, unityPart2>(elem, countPart2);
        }
      } else if (y1 == y2) {
        // horizontal line
        for (size_t x = std::min(x1, x2); x <= std::max(x1, x2); ++x) {
          auto& elem = board[y1 * dim + x];
          increment<maskPart1, unityPart1>(elem, countPart1);
          increment<maskPart2, unityPart2>(elem, countPart2);
        }
      } else {
        // We separate these case for an easy end condition
        if (x1 < x2) {
          const int16_t stepX = 1;
          const int16_t stepY = (y1 < y2) - (y1 > y2);
          for (int16_t x = x1, y = y1; x <= x2; x += stepX, y += stepY) {
            auto& elem = board[y * dim + x];
            increment<maskPart2, unityPart2>(elem, countPart2);
          }
        } else {
          const int16_t stepX = -1;
          const int16_t stepY = (y1 < y2) - (y1 > y2);
          for (int16_t x = x1, y = y1; x >= x2; x += stepX, y += stepY) {
            auto& elem = board[y * dim + x];
            increment<maskPart2, unityPart2>(elem, countPart2);
          }
        }
      }
    }
    return {countPart1, countPart2};
});


} // namespace aoc
