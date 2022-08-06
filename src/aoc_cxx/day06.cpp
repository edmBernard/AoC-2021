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
    fmt::print("{} ", mark[col]);
  }
  fmt::print("\n");
}

// Structure that memoize result of the computation
struct Engine {

  Engine(size_t epochByStep, size_t numberOfStep)
  : epochByStep(epochByStep), numberOfStep(numberOfStep), cacheComputedTotal(numberOfStep+1, std::vector<uint64_t>(9, 0)) {

    preComputeNextPopulation(epochByStep);
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

  void preComputeNextPopulation(uint16_t epoch) {
    // compute the population create by each state fish
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


const RegisterCommand day06("day06", {
    { "input_day06.txt",       345793,  1572643095893},
    { "input_day06_test1.txt", 5934,    26984457539},
  }, [](fs::path filename) -> std::array<uint64_t, 2> {

    std::ifstream infile(filename);
    if (!infile.is_open()) {
      throw std::runtime_error(fmt::format("File Not Found : {}", filename.string()));
    }

    std::string line;

    getline(infile, line);
    std::vector<uint8_t> population = aoc::splitString(line, ',');

    // part1
    Engine enginePart1(40, 80 / 40 -1);
    uint64_t countPart1 = enginePart1.computeTotal(population);

    // part2
    Engine enginePart2(32, 256 / 32 -1);
    uint64_t countPart2 = enginePart2.computeTotal(population);

    return {countPart1, countPart2};
});


} // namespace aoc
