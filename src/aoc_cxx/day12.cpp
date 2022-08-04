#include "controller.hpp"
#include "utils.hpp"

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

uint64_t graphWalk(uint8_t currentNode, const std::vector<uint8_t>& counterSmallCave, uint8_t indexSmallCave, const std::vector<std::vector<uint8_t>>& puzzleInputInVector, bool isPart1) {
  // check if small cave
  if (currentNode >= indexSmallCave) {
    if (counterSmallCave[currentNode - indexSmallCave] > 0) {
      if (isPart1) {
        return 0;
      } else {
        for (auto value : counterSmallCave) {
          if (value >= 2) {
            return 0;
          }
        }
      }
    }
    // try to reduce allocation. counterSmallCave is modified/copied only for small cave
    std::vector<uint8_t> counterSmallCaveCopy = counterSmallCave;
    counterSmallCaveCopy[currentNode - indexSmallCave] += 1;
    uint64_t numberPath = 0;
    for (auto v : puzzleInputInVector[currentNode]) {
      switch (v) {
        case 0: // 0 is the Start Node
          continue;
        case 1: // 1 is the End Node
          numberPath += 1;
          break;
        default:
          numberPath += graphWalk(v, counterSmallCaveCopy, indexSmallCave, puzzleInputInVector, isPart1);
      }
    }
    return numberPath;

  } else {

    uint64_t numberPath = 0;
    for (auto v : puzzleInputInVector[currentNode]) {
      switch (v) {
        case 0: // 0 is the Start Node
          continue;
        case 1: // 1 is the End Node
          numberPath += 1;
          break;
        default:
          numberPath += graphWalk(v, counterSmallCave, indexSmallCave, puzzleInputInVector, isPart1);
      }
    }
    return numberPath;
  }
}

} // namespace


RegisterCommand day12("day12", {
    { "input_day12.txt",       3713,  91292},
    { "input_day12_test1.txt", 10,    36},
    { "input_day12_test2.txt", 226,   3509},
  }, [](fs::path filename) -> std::array<uint64_t, 2> {

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
    // convert map to vector
    std::unordered_map<std::string, uint8_t> mapNodeIndex;
    mapNodeIndex["start"] = 0;
    mapNodeIndex["end"] = 1;

    uint8_t count = 2;
    // big cave
    for (auto& [key, value] : puzzleInput) {
      if (key[0] < 97) {
        mapNodeIndex[key] = count;
        ++count;
      }
    }
    // small cave
    uint8_t indexSmallCave = count;
    for (auto& [key, value] : puzzleInput) {
      if (key[0] >= 97 && key != "start" && key != "end") {
        mapNodeIndex[key] = count;
        ++count;
      }
    }
    std::vector<std::vector<uint8_t>> puzzleInputInVector(count, std::vector<uint8_t>{});
    for (auto& [key, value] : puzzleInput) {
      for (auto& v : value) {
        puzzleInputInVector[mapNodeIndex[key]].push_back(mapNodeIndex[v]);
      }
    }
    std::vector<uint8_t> counterSmallCave(puzzleInputInVector.size() - indexSmallCave, 0);
    uint64_t countPart1 = graphWalk(0, counterSmallCave, indexSmallCave, puzzleInputInVector, true);
    uint64_t countPart2 = graphWalk(0, counterSmallCave, indexSmallCave, puzzleInputInVector, false);

    return {countPart1, countPart2};
});


} // namespace aoc
