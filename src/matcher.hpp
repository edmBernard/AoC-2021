
#pragma once

#include <spdlog/spdlog.h>

#include <string>
#include <string_view>
#include <vector>
#include <sstream>
#include <algorithm>

namespace aoc {

namespace utils {

inline std::string join(const std::vector<std::string> &tokens, const std::string &delimiter) {
  std::stringstream stream;
  stream << tokens.front();
  std::for_each(begin(tokens) + 1, end(tokens),
                [&](const std::string &elem) { stream << delimiter << elem; });
  return stream.str();
}

inline std::vector<std::string> split(std::string const &original, char separator) {
  std::vector<std::string> results;
  std::string::const_iterator start = original.begin();
  std::string::const_iterator end = original.end();
  std::string::const_iterator next = std::find(start, end, separator);

  while (next != end) {
    results.push_back(std::string(start, next));
    start = next + 1;
    next = std::find(start, end, separator);
  }

  results.push_back(std::string(start, next));

  return results;
}

inline bool match(std::string_view name, std::string_view filter) {
  if (name.size() < filter.size()) {
    return false;
  }
  for (std::size_t i = 0; i < filter.size(); ++i) {
    if (name[i] != filter[i]) {
      return false;
    }
  }
  return true;
}
} // namespace utils
inline bool matcher(std::string_view name, std::string_view filter) {

  if (filter.empty()) {
    return true;
  }
  const bool negative = filter[0] == '!';
  const size_t offset = negative ? 1 : 0;

  bool doesMatch = false;
  for (auto& tag : utils::split(std::string(name), ',')) {
    spdlog::debug("name : {} processed tag : {}", name, tag);

    if (name.find(filter.data() + offset) != std::string::npos) {
      doesMatch = true;
    }
  }
  return negative ? !doesMatch : doesMatch;
}

} // namespace aoc
