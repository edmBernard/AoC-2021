#include "controller.hpp"
#include "utils.hpp"

#include "aoc_rust.h"

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

uint64_t graphWalk(const std::string& currentNode, std::vector<std::string> path, const std::unordered_map<std::string, std::vector<std::string>>& puzzleInput) {
  if (currentNode == "end") {
    // showVect(path);
    return 1;
  }

  // check if small cave
  // 65 is 'A' and 97 'a'
  // >= 97 check if it's minuscule
  if (currentNode[0] >= 97) {
    auto pos = std::find(path.begin(), path.end(), currentNode);
    if (pos != path.end()) {
      return 0;
    }
  }

  uint64_t count = 0;
  path.push_back(currentNode);
  for (const auto& v : puzzleInput.at(currentNode)) {
    count += graphWalk(v, path, puzzleInput);
  }
  return count;
}

} // namespace


RegisterCommand day12("day12", {
    { "input_day12.txt",       1719,  232},
    { "input_day12_test1.txt", 226,  195},
  }, [](fs::path filename) -> std::tuple<uint64_t, uint64_t> {

    std::ifstream infile(filename);
    if (!infile.is_open()) {
      throw std::runtime_error(fmt::format("File Not Found : {}", filename.string()));
    }

    std::string line;
    std::unordered_map<std::string, std::vector<std::string>> puzzleInput;

    while (getline(infile, line)) {
      std::vector<std::string> temp = aoc::splitString(line, '-');
      puzzleInput[temp[0]].push_back(temp[1]);
      puzzleInput[temp[1]].push_back(temp[0]);
    }

    // for (auto& [key, value] : puzzleInput) {
    //   fmt::print("key={}\n", key);
    //   for (auto& v : value) {
    //     fmt::print("  value={}\n", v);
    //   }
    // }


    uint64_t countPart1 = graphWalk("start", {}, puzzleInput);
    uint64_t countPart2 = 0;


    return {countPart1, countPart2};
});


} // namespace aoc
