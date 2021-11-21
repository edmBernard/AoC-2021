
#pragma once

#include <matcher.hpp>

#include <spdlog/spdlog.h>

#include <functional>
#include <string>

namespace aoc {

using CommandFunction = std::function<int(int, int)>;

class Controller {
public:
  Controller(std::string filter)
      : commands(GetCommandRegister()), filter(filter) {
    spdlog::debug("Number of command registered : {}", commands.size());
  }

  void run() {
    for (auto &[name, command] : commands) {
      if (!matcher(name, filter)) {
        continue;
      }
      spdlog::info("{}", name);
    }
  }

  void show() {

    for (auto &[name, command] : commands) {
      spdlog::info("{}", name);
    }
  }

  static std::vector<std::tuple<std::string, CommandFunction>> &GetCommandRegister() {
    static std::vector<std::tuple<std::string, CommandFunction>> g_command;
    return g_command;
  }

private:
  const std::vector<std::tuple<std::string, CommandFunction>> commands;
  const std::string filter;
};

class RegisterCommand {
public:
  RegisterCommand(const std::string &name, CommandFunction command) {
    Controller::GetCommandRegister().push_back({name, command});
  }
};

} // namespace aoc