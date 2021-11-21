
#pragma once

#include <matcher.hpp>

#include <spdlog/spdlog.h>

#include <functional>
#include <string>
#include <vector>
#include <tuple>
#include <filesystem>


namespace aoc {

namespace fs = std::filesystem;

using CommandFunction = std::function<std::tuple<size_t, size_t>(fs::path)>;
using ExpectedResults = std::vector<std::tuple<fs::path, size_t, size_t>>;
using CommandRegister = std::vector<std::tuple<std::string, ExpectedResults, CommandFunction>>;

class Controller {
public:
  Controller(std::string filter)
      : commands(GetCommandRegister()), filter(filter) {
    spdlog::debug("Number of command registered : {}", commands.size());
  }

  void run(fs::path input) {
    if (!fs::exists(input)) {
      throw std::runtime_error(fmt::format("Input path does not exists : {}", fs::absolute(input).string()));
    }

    const bool isDirectory = fs::is_directory(input);
    for (auto &[name, expectedResults, command] : commands) {
      if (!matcher(name, filter)) {
        continue;
      }
      if (expectedResults.empty() && isDirectory) {
        continue;
      }

      if (expectedResults.empty() || !isDirectory) {
        auto start_temp = std::chrono::high_resolution_clock::now();

        auto [part1, part2] = command(input);

        std::chrono::duration<double, std::milli> elapsed_temp = std::chrono::high_resolution_clock::now() - start_temp;                                                                           \
        spdlog::info("{: <10} in {:>7.2f} ms : part1={:<10} part2={:<10}", name, elapsed_temp.count(), part1, part2);
        continue;
      }

      for (auto& [filename, expectedPart1, expectedPart2] : expectedResults) {
        auto start_temp = std::chrono::high_resolution_clock::now();

        auto [part1, part2] = command(input / filename);

        std::chrono::duration<double, std::milli> elapsed_temp = std::chrono::high_resolution_clock::now() - start_temp;                                                                           \
        spdlog::info("{: <10} in {:>7.2f} ms : part1={:<10} part2={:<10}", name, elapsed_temp.count(), part1, part2);

        if (part1 != expectedPart1) {
          spdlog::error("Result Part1 missmatch : expected={:<10}  got={:<10}", expectedPart1, part1);
        }
        if (part2 != expectedPart2) {
          spdlog::error("Result Part2 missmatch : expected={:<10}  got={:<10}", expectedPart2, part2);
        }
      }
    }
  }

  void show() {

    for (auto &[name, defaultFilename, command] : commands) {
      spdlog::info("{}", name);
    }
  }

  static CommandRegister &GetCommandRegister() {
    static CommandRegister g_command;
    return g_command;
  }

private:
  const CommandRegister commands;
  const std::string filter;
};

class RegisterCommand {
public:
  RegisterCommand(const std::string &name, ExpectedResults expectedResults, CommandFunction command) {
    Controller::GetCommandRegister().push_back({name, expectedResults, command});
  }
};

} // namespace aoc