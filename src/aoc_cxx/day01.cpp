#include "controller.hpp"

#include "aoc_rust.h"

#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>
#include <charconv>

#include <range/v3/all.hpp>

namespace aoc {

namespace fs = std::filesystem;
namespace rs = ranges;
namespace rv = ranges::views;


RegisterCommand day01("day01", {
    { "input_day01.txt",       1233,  1275},
    { "input_day01_test1.txt", 7,     5},
  }, [](fs::path filename) -> std::tuple<uint64_t, uint64_t> {

    std::ifstream infile(filename);
    if (!infile.is_open()) {
      throw std::runtime_error(fmt::format("File Not Found : {}", filename.string()));
    }

    std::vector<uint16_t> depthList;
    std::string line;
    // getline(infile, line) is faster than infile >> line
    // from_char is faster than stol
    // istream_iterator<uint16_t> is also slower than this while loop
    while (getline(infile, line)) {
      uint16_t result;
      const auto [ptr, ec] = std::from_chars(line.data(), line.data() + line.size(), result);
      if (ec != std::errc())
        throw std::runtime_error(fmt::format("Fail to parse : {}", line));
      depthList.push_back(result);
    }

    // part1
    uint16_t part1Result = 0;
    for (int idx = 1; idx < depthList.size(); ++idx) {
      if (depthList[idx] > depthList[idx-1]) {
        ++part1Result;
      }
    }

    // part2
    uint16_t part2Result = 0;
    uint16_t previous = depthList[0] + depthList[1] + depthList[2];
    for (int idx = 2; idx < depthList.size(); ++idx) {
      const uint16_t sum = depthList[idx] + depthList[idx-1] + depthList[idx-2];
      if (sum > previous) {
        ++part2Result;
      }
      previous = sum;
    }
    return {part1Result, part2Result};
});


RegisterCommand day01simple("day01,simple", {
    { "input_day01.txt",       1233,  1275},
    { "input_day01_test1.txt", 7,     5},
  }, [](fs::path filename) -> std::tuple<uint64_t, uint64_t> {

    std::ifstream infile(filename);
    if (!infile.is_open()) {
      throw std::runtime_error(fmt::format("File Not Found : {}", filename.string()));
    }

    std::istream_iterator<uint16_t> start(infile), end;
    std::vector<uint16_t> depthList(start, end);

    // part1
    uint16_t part1Result = 0;
    for (int idx = 1; idx < depthList.size(); ++idx) {
      if (depthList[idx] > depthList[idx-1]) {
        ++part1Result;
      }
    }

    // part2
    uint16_t part2Result = 0;
    uint16_t previous = depthList[0] + depthList[1] + depthList[2];
    for (int idx = 2; idx < depthList.size(); ++idx) {
      const uint16_t sum = depthList[idx] + depthList[idx-1] + depthList[idx-2];
      if (sum > previous) {
        ++part2Result;
      }
      previous = sum;
    }
    return {part1Result, part2Result};
});


RegisterCommand day01range("day01,range", {
    { "input_day01.txt",       1233,  1275},
    { "input_day01_test1.txt", 7,     5},
  }, [](fs::path filename) -> std::tuple<uint64_t, uint64_t> {

    std::ifstream infile(filename);
    if (!infile.is_open()) {
      throw std::runtime_error(fmt::format("File Not Found : {}", filename.string()));
    }

    const std::vector<uint16_t> depthList = rs::istream_view<uint16_t>(infile) | rs::to<std::vector>;

    // part1
    // note: transform + accumulate seem to be faster than filter + distance. Probably due to the size change from filter
    const uint64_t part1Result = rs::accumulate(
        depthList
        | rv::sliding(2)
        | rv::transform([](auto&& window) { return window[0] < window[1] ? 1 : 0; }), 0);

    // part2
    const uint64_t part2Result = rs::accumulate(
        depthList
        | rv::sliding(3)
        | rv::transform([](auto&& window) { return rs::accumulate(window, 0); })
        | rv::sliding(2)
        | rv::transform([](auto&& window) { return window[0] < window[1] ? 1 : 0; }), 0);

    return {part1Result, part2Result};
});


RegisterRustCommand day01rust("day01,rust", {
    { "input_day01.txt",       1233,  1275},
    { "input_day01_test1.txt", 7,     5},
  }, rust::day01);


RegisterRustCommand day01rustfunctional("day01,rust,functional", {
    { "input_day01.txt",       1233,  1275},
    { "input_day01_test1.txt", 7,     5},
  }, rust::day01functional);

} // namespace aoc