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

template <typename T>
struct Board {
  Board(const std::vector<T> &data, size_t width, size_t height) : data(data), width(width), height(height) {}

  T operator()(int64_t x, int64_t y) {
    if (x < 0)
      x = 0;
    if (y < 0)
      y = 0;
    if (x >= width)
      x = width - 1;
    if (y >= height)
      y = height - 1;
    return data[y * width + x];
  }
  bool isLocalMin(int64_t x, int64_t y) {
    int count = 4;
    if (x == 0)
      --count;
    if (y == 0)
      --count;
    if (y == height - 1)
      --count;
    if (x == width - 1)
      --count;
    count -= operator()(x, y) < operator()(x + 1, y);
    count -= operator()(x, y) < operator()(x - 1, y);
    count -= operator()(x, y) < operator()(x, y + 1);
    count -= operator()(x, y) < operator()(x, y - 1);
    return count == 0;
  }
  const std::vector<T> &data;
  const uint64_t width;
  const uint64_t height;
};

} // namespace


RegisterCommand day09("day09", {
    { "input_day09.txt",       495, 1055164},
    { "input_day09_test1.txt", 26,  61229},
  }, [](fs::path filename) -> std::tuple<uint64_t, uint64_t> {

    std::ifstream infile(filename);
    if (!infile.is_open()) {
      throw std::runtime_error(fmt::format("File Not Found : {}", filename.string()));
    }

    std::string line;
    std::vector<uint8_t> puzzleInput;
    uint64_t width = 0;
    uint64_t height = 0;

    while (getline(infile, line)) {
      if (width == 0)
        width = line.size();

      for (char c : line) {
        puzzleInput.push_back(static_cast<uint16_t>(c - '0'));
      }
      height += 1;
    }

    showBoard(puzzleInput, {width, height});
    // part1
    Board board(puzzleInput, width, height);
    uint64_t countPart1 = 0;
    uint64_t minCount = 0;
    for (size_t row = 0; row < height; ++row) {
      for (size_t col = 0; col < width; ++col) {
        if (board.isLocalMin(col, row)) {
          // fmt::print("min x={} y={}\n", col, row);
          minCount += 1;
          countPart1 = countPart1 + 1 + board(col, row);
        }
      }
    }
    // fmt::print("min count {}\n", minCount);

    // part2
    uint64_t countPart2 = 0;

    return {countPart1, countPart2};
});


} // namespace aoc
