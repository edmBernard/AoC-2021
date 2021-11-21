#include "controller.hpp"

#include <spdlog/spdlog.h>

#include <filesystem>

namespace aoc {

namespace fs = std::filesystem;

RegisterCommand add("add", "aocday1.txt", [](int a, int b, fs::path directory, fs::path filename) {
  spdlog::debug("directory: {}", directory.string());
  spdlog::debug("filename: {}", filename.string());
  return a + b;
});

RegisterCommand addTest("add,test", "aocday1.txt", [](int a, int b, fs::path directory, fs::path filename) {
  return a + b;
});

RegisterCommand mul("mul", "aocday1.txt", [](int a, int b, fs::path directory, fs::path filename) {
  return a * b;
});

RegisterCommand sub("sub", "aocday1.txt", [](int a, int b, fs::path directory, fs::path filename) {
  return a - b;
});

RegisterCommand mydiv("mydiv", "aocday1.txt", [](int a, int b, fs::path directory, fs::path filename) {
  return a / b;
});

} // namespace aoc