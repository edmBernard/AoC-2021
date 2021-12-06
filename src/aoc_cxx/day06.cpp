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

} // namespace


RegisterCommand day06("day06", {
    { "input_day06.txt",       345793,  21373},
    { "input_day06_test1.txt", 5934,     12},
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

    const uint8_t numberDay = 80;
    for (uint8_t i = 0; i < numberDay; ++i) {
      uint64_t toAdd = 0;
      std::transform(population.begin(), population.end(), population.begin(), [&toAdd](auto x) {
          toAdd += (x == 0);
          return x == 0 ? 6 : x - 1;
        });
      population.resize(population.size() + toAdd, 8);  // C++20
      // showVect(population);
    }

    return {population.size(), 0};
});


} // namespace aoc
