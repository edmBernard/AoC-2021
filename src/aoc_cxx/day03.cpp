#include "controller.hpp"

#include "aoc_rust.h"

#include <range/v3/all.hpp>
#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>
#include <vector>
#include <tuple>
#include <charconv>

namespace aoc {

namespace fs = std::filesystem;
namespace rs = ranges;
namespace rv = ranges::views;


RegisterCommand day03("day03", {
    { "input_day03.txt",       3429254,   0},
    { "input_day03_test1.txt", 198,       0},
  }, [](fs::path filename) -> std::tuple<uint64_t, uint64_t> {

    std::ifstream infile(filename);
    if (!infile.is_open()) {
      throw std::runtime_error(fmt::format("File Not Found : {}", filename.string()));
    }

    std::vector<uint64_t> occurenceOf0;
    std::vector<uint64_t> occurenceOf1;
    std::string line;
    while (getline(infile, line)) {
      const size_t lineLength = line.size();
      if (occurenceOf1.empty()) {
        occurenceOf0 = std::vector<uint64_t>(lineLength, 0);
        occurenceOf1 = std::vector<uint64_t>(lineLength, 0);
      }

      for (size_t i = 0; i < line.size(); ++i) {
        if (line[i] == '0') {
          occurenceOf0[i] += 1;
        } else {
          occurenceOf1[i] += 1;
        }
      }
    }

    uint64_t gammaRate = 0;
    uint64_t epsilonRate = 0;
    for (int64_t i = 0, shift = occurenceOf0.size() -1; i < occurenceOf0.size(); ++i, --shift) {
      gammaRate += (occurenceOf0[i] > occurenceOf1[i] ? 0 : 1) << shift;
      epsilonRate += (occurenceOf0[i] > occurenceOf1[i] ? 1 : 0) << shift;
    }
    const uint64_t part1Result = gammaRate * epsilonRate;

    return {part1Result, 0};
});

} // namespace aoc