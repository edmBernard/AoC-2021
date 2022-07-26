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


template <typename T>
void showVect(const std::vector<T> &mark) {
  for (size_t col = 0; col < mark.size(); ++col) {
    fmt::print("'{}' ", mark[col]);
  }
  fmt::print("\n");
}


template <typename T>
void showBoard(const std::vector<T> &mark, std::array<uint64_t, 2> size) {
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
  Board(std::vector<T> &data, size_t width, size_t height) : data(data), width(width), height(height) {}

  // return the number of flash
  uint64_t increment(int64_t x, int64_t y) {
    if (x < 0 || y < 0 || x >= width || y >= height)
      return 0;

    ++data[y * width + x];
    if (data[y * width + x] != 10)
      return 0;

    uint64_t total = 1;
    total += increment(x-1, y-1);
    total += increment(x,   y-1);
    total += increment(x+1, y-1);
    total += increment(x-1, y);
    total += increment(x+1, y);
    total += increment(x-1, y+1);
    total += increment(x,   y+1);
    total += increment(x+1, y+1);
    return total;
  }

  bool reset(int64_t x, int64_t y) {
    if (x < 0 || y < 0 || x >= width || y >= height)
      return false;

    if (data[y * width + x] > 9) {
      data[y * width + x] = 0;
      return true;
    }
    return false;
  }

  std::vector<T> &data;
  const uint64_t width;
  const uint64_t height;
};

} // namespace


RegisterCommand day11("day11", {
    { "input_day11.txt",       1719,  232},
    { "input_day11_test1.txt", 1656,  195},
  }, [](fs::path filename) -> std::array<uint64_t, 2> {

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

    Board board(puzzleInput, width, height);

    uint64_t countPart1 = 0;
    uint64_t countPart1Temp = 0;

    uint64_t countPart2 = 0;
    int step = 0;
    while (true) {
      uint64_t numberFlash = 0;
      for (size_t row = 0; row < height; ++row) {
        for (size_t col = 0; col < width; ++col) {
          countPart1Temp += board.increment(col, row);
        }
      }
      for (size_t row = 0; row < height; ++row) {
        for (size_t col = 0; col < width; ++col) {
          numberFlash += board.reset(col, row);
        }
      }
      ++step;
      if (step == 100) {
        countPart1 = countPart1Temp;
      }
      if (countPart2 == 0 && numberFlash == 100) {
        countPart2 = step;
        break;
      }
    }

    return {countPart1, countPart2};
});


} // namespace aoc
