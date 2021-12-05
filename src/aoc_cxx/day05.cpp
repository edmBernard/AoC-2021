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
inline T parse(std::string_view input, int base = 10) {
  T result;
  const auto [ptr, ec] = std::from_chars(input.data(), input.data() + input.size(), result, base);
  if (ec != std::errc())
    throw std::runtime_error(fmt::format("Fail to parse : {}", input));
  return result;
}


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


} // namespace

RegisterCommand day05("day05", {
    { "input_day05.txt",       7380,  16836},
    { "input_day05_test1.txt", 5,     1924},
  }, [](fs::path filename) -> std::tuple<uint64_t, uint64_t> {

    std::ifstream infile(filename);
    if (!infile.is_open()) {
      throw std::runtime_error(fmt::format("File Not Found : {}", filename.string()));
    }

    std::string line;
    std::vector<std::array<uint16_t, 4>> inputPuzzle;

    while (getline(infile, line)) {

      std::vector<std::string> splitted = split(line, ' ');
      std::vector<std::string> left = split(splitted[0], ',');
      std::vector<std::string> right = split(splitted[2], ',');

      inputPuzzle.push_back({parse<uint16_t>(left[0]), parse<uint16_t>(left[1]), parse<uint16_t>(right[0]), parse<uint16_t>(right[1])});

    }

    constexpr uint16_t dim = 1000;
    std::vector<uint16_t> board(dim*dim, 0);
    for (auto& [x1, y1, x2, y2] : inputPuzzle) {
      if (x1 == x2) {
        for (size_t i = std::min(y1, y2); i <= std::max(y1, y2); ++i) {
          board[i * dim + x1] += 1;
        }
      }
      if (y1 == y2) {
        for (size_t i = std::min(x1, x2); i <= std::max(x1, x2); ++i) {
          board[y1 * dim + i] += 1;
        }
      }
    }
    const uint64_t countPart1 = std::count_if(board.begin(), board.end(), [](auto x){ return x > 1; });

    // showBoard<dim>(board);
    return {countPart1, 0};
});


} // namespace aoc
