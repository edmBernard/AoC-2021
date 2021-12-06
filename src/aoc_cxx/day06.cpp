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

std::vector<uint8_t> getFinalPopulation(uint8_t initial, uint16_t epoch) {
  std::vector<uint8_t> population{initial};
  for (uint16_t i = 0; i < epoch; ++i) {
    uint64_t toAdd = 0;
    std::transform(population.begin(), population.end(), population.begin(), [&toAdd](auto x) {
        toAdd += (x == 0);
        return x == 0 ? 6 : x - 1;
      });
    population.resize(population.size() + toAdd, 8);  // C++20
  }
  return population;
}

uint64_t getCountPopulation(uint8_t initial, uint16_t epoch) {
  std::vector<uint8_t> population{initial};
  for (uint16_t i = 0; i < epoch; ++i) {
    uint64_t toAdd = 0;
    std::transform(population.begin(), population.end(), population.begin(), [&toAdd](auto x) {
        toAdd += (x == 0);
        return x == 0 ? 6 : x - 1;
      });
    population.resize(population.size() + toAdd, 8);  // C++20
  }
  return population.size();
}

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
    std::vector<uint64_t> preComputeCountPart1Step80;
    for (int i = 0; i <=8; ++i) {
      preComputeCountPart1Step80.push_back(getCountPopulation(i, 80));
    }
    uint64_t countPart1 = 0;
    for (auto& v : population) {
      countPart1 += preComputeCountPart1Step80[v];
    }

    // part2
    std::vector<std::vector<uint8_t>> preComputePopulationPart2Step128;
    std::vector<uint64_t> preComputeCountPart2Step128;
    for (int i = 0; i <=8; ++i) {
      const std::vector<uint8_t> pop = getFinalPopulation(i, 128);
      preComputeCountPart2Step128.push_back(pop.size());
      preComputePopulationPart2Step128.push_back(pop);
    }
    uint64_t countPart2 = 0;
    for (auto& v : population) {
      auto pop = preComputePopulationPart2Step128[v];
      for (auto& p : pop) {
        countPart2 += preComputeCountPart2Step128[p];
      }
    }

    return {countPart1, countPart2};
});


} // namespace aoc
