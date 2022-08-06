#include "controller.hpp"

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
  std::vector<T> &data;
  const uint64_t width;
  const uint64_t height;
};

} // namespace


const RegisterCommand day09("day09", {
    { "input_day09.txt",       564, 1038240},
    { "input_day09_test1.txt", 15,  1134},
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
    std::vector<int16_t> basinData(puzzleInput.size(), 0);
    Board basin(basinData, width, height);

    uint64_t countPart1 = 0;
    uint64_t minCount = 0;
    for (size_t row = 0; row < height; ++row) {
      for (size_t col = 0; col < width; ++col) {
        if (board.isLocalMin(col, row)) {
          // fmt::print("min x={} y={}\n", col, row);
          minCount += 1;
          countPart1 = countPart1 + 1 + board(col, row);
          basinData[row * width + col] = minCount;
        }
        if (board(col, row) == 9)
          basinData[row * width + col] = -1;
      }
    }

    std::vector<uint16_t> basinSize(minCount, 1);

    while (std::count(basinData.begin(), basinData.end(), 0) != 0) {
      for (size_t row = 0; row < height; ++row) {
        for (size_t col = 0; col < width; ++col) {
          int16_t& elem = basinData[row * width + col];

          if (elem != 0)
            continue;

          if (basin(col+1, row) > 0) {
            elem = basin(col+1, row);
            ++basinSize[elem-1];
            continue;
          }
          if (basin(col-1, row) > 0) {
            elem = basin(col-1, row);
            ++basinSize[elem-1];
            continue;
          }
          if (basin(col, row+1) > 0) {
            elem = basin(col, row+1);
            ++basinSize[elem-1];
            continue;
          }
          if (basin(col, row-1) > 0) {
            elem = basin(col, row-1);
            ++basinSize[elem-1];
            continue;
          }
        }
      }
    }

    std::sort(basinSize.rbegin(), basinSize.rend());

    uint64_t countPart2 = basinSize[0] * basinSize[1] * basinSize[2];

    return {countPart1, countPart2};
});


} // namespace aoc
