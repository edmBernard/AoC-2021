#include "controller.hpp"

#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>

namespace aoc {

namespace fs = std::filesystem;

RegisterCommand day01("day01", {
    { "day01.txt",              381699,   111605670},
    { "day01_part1_test1.txt",  514579,   241861950},
  }, [](fs::path filename) -> std::tuple<size_t, size_t> {

    std::ifstream infile(filename);
    if (!infile.is_open()) {
      throw std::runtime_error(fmt::format("File Not Found : {}", filename.string()));
    }

    std::vector<size_t> numberList;
    std::string line;
    while (infile >> line) {
      numberList.push_back(std::stoi(line));
    }

    long part1Result;
    long part2Result;
    // part1
    for (auto l1 : numberList) {
      for (auto l2 : numberList) {
        if (l1 + l2 == 2020) {
          part1Result = l1 * l2;
        }
      }
    }

    // part2
    for (auto l1 : numberList) {
      for (auto l2 : numberList) {
        for (auto l3 : numberList) {
          if (l1 + l2 + l3 == 2020) {
            part2Result = l1 * l2 * l3;
          }
        }
      }
    }

    spdlog::debug("filename: {}", filename.string());
    return {part1Result, part2Result};
});

} // namespace aoc