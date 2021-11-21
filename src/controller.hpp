
#pragma once

#include <matcher.hpp>

#include <spdlog/spdlog.h>

#include <functional>
#include <string>
#include <filesystem>

namespace aoc {

namespace fs = std::filesystem;


using CommandFunction = std::function<std::tuple<size_t, size_t>(fs::path)>;
using CommandRegister = std::vector<std::tuple<std::string, fs::path, CommandFunction>>;

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
    for (auto &[name, defaultFilename, command] : commands) {
      if (!matcher(name, filter)) {
        continue;
      }
      fs::path finalInputPath = isDirectory ? input / defaultFilename : input;

      auto start_temp = std::chrono::high_resolution_clock::now();

      auto [part1, part2] = command(finalInputPath);

      std::chrono::duration<double, std::milli> elapsed_temp = std::chrono::high_resolution_clock::now() - start_temp;                                                                           \
      spdlog::info("{: <10} in {:>7.2f} ms : part1={:<40} part2={:<40}", name, elapsed_temp.count(), part1, part2);
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
  RegisterCommand(const std::string &name, fs::path defaultFilename, CommandFunction command) {
    Controller::GetCommandRegister().push_back({name, defaultFilename, command});
  }
};

} // namespace aoc