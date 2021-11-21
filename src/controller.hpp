
#pragma once

#include <matcher.hpp>

#include <spdlog/spdlog.h>

#include <functional>
#include <string>
#include <filesystem>

namespace aoc {

namespace fs = std::filesystem;


using CommandFunction = std::function<int(int, int, fs::path, fs::path)>;

class Controller {
public:
  Controller(std::string filter)
      : commands(GetCommandRegister()), filter(filter) {
    spdlog::debug("Number of command registered : {}", commands.size());
  }

  void run(fs::path directory, fs::path filename) {
    for (auto &[name, defaultFilename, command] : commands) {
      if (!matcher(name, filter)) {
        continue;
      }
      if (filename.empty()) {
        command(1, 2, directory, defaultFilename);
      } else {
        command(1, 2, directory, filename);
      }
      spdlog::info("{}", name);
    }
  }

  void show() {

    for (auto &[name, defaultFilename, command] : commands) {
      spdlog::info("{}", name);
    }
  }

  static std::vector<std::tuple<std::string, fs::path, CommandFunction>> &GetCommandRegister() {
    static std::vector<std::tuple<std::string, fs::path, CommandFunction>> g_command;
    return g_command;
  }

private:
  const std::vector<std::tuple<std::string, fs::path, CommandFunction>> commands;
  const std::string filter;
};

class RegisterCommand {
public:
  RegisterCommand(const std::string &name, fs::path defaultFilename, CommandFunction command) {
    Controller::GetCommandRegister().push_back({name, defaultFilename, command});
  }
};

} // namespace aoc