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
    fmt::print("{} ", mark[col]);
  }
  fmt::print("\n");
}

// Structure that memoize result of the computation
struct Engine {

  Engine(size_t epochByStep, size_t numberOfStep)
  : epochByStep(epochByStep), numberOfStep(numberOfStep), cacheComputedTotal(numberOfStep+1, std::vector<uint64_t>(9, 0)) {

    preComputeResult(epochByStep);
  }

  uint64_t computeTotal(const std::vector<uint8_t>& initial) {
    uint64_t total = 0;
    for (auto& p : initial) {
      total += computeTotal(p, 0);
    }
    return total;
  }

private:

  // Compute total by recursion
  uint64_t computeTotal(uint8_t initial, uint16_t step) {
    auto& result = cacheComputedTotal[step][initial];
    if (result != 0)
      return result;

    // if no cache found compute it
    std::vector<uint8_t> population = cacheComputedPopulation[initial];

    if (step >= numberOfStep)
      return population.size();

    for (auto& p : population)
      result += computeTotal(p, step + 1);

    return result;
  }

  void preComputeResult(uint16_t epoch) {
    for (uint8_t i = 0; i <= 8; ++i) {
      std::vector<uint8_t> population{i};
      for (uint16_t i = 0; i < epoch; ++i) {
        uint64_t toAdd = 0;
        std::transform(population.begin(), population.end(), population.begin(), [&toAdd](auto x) {
            toAdd += (x == 0);
            return x == 0 ? 6 : x - 1;
          });
        population.resize(population.size() + toAdd, 8);  // C++20
      }
      cacheComputedPopulation.push_back(population);
    }
  }

  size_t epochByStep;
  size_t numberOfStep;
  std::vector<std::vector<uint8_t>> cacheComputedPopulation;
  std::vector<std::vector<uint64_t>> cacheComputedTotal;
};


} // namespace


RegisterCommand day06("day06", {
    { "input_day06.txt",       345793,  1572643095893},
    { "input_day06_test1.txt", 5934,    26984457539},
  }, [](fs::path filename) -> std::tuple<uint64_t, uint64_t> {

    std::ifstream infile(filename);
    if (!infile.is_open()) {
      throw std::runtime_error(fmt::format("File Not Found : {}", filename.string()));
    }

    std::string line;
    std::vector<uint8_t> population;

    while (getline(infile, line)) {
      population = parse<uint8_t>(split(line, ','));
    }

    // part1
    Engine engine1(40, 80 / 40 -1);
    uint64_t countPart1 = engine1.computeTotal(population);

    // part2
    Engine engine2(32, 256 / 32 -1);
    uint64_t countPart2 = engine2.computeTotal(population);

    return {countPart1, countPart2};
});


} // namespace aoc
