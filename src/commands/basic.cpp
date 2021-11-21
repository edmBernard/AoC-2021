#include "controller.hpp"

namespace aoc {

RegisterCommand add("add", [](int a, int b) {
  return a + b;
});

RegisterCommand addTest("add,test", [](int a, int b) {
  return a + b;
});

RegisterCommand mul("mul", [](int a, int b) {
  return a * b;
});

RegisterCommand sub("sub", [](int a, int b) {
  return a - b;
});

RegisterCommand mydiv("mydiv", [](int a, int b) {
  return a / b;
});

} // namespace aoc