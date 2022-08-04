

#pragma once

#include <fmt/core.h>

#include <algorithm>
#include <charconv>
#include <string>
#include <vector>


namespace aoc {

// String Helpers
namespace details {
  template <typename T, typename Splitter, typename Enable = void>
  struct ParseWithSplitter {
    ParseWithSplitter(Splitter splitter)
        : splitter(splitter) {
    }

    std::vector<T> apply() {
      std::vector<T> out;
      // Debug<T>();
      while (splitter.finished != true) {
        out.emplace_back(splitter.getNext());
      }
      return out;
    }

    Splitter splitter;
  };

  template <typename T, typename Splitter>
  struct ParseWithSplitter<T, Splitter, typename std::enable_if<std::is_floating_point_v<T>>::type> {
    ParseWithSplitter(Splitter splitter)
        : splitter(splitter) {
    }

    std::vector<T> apply() {
      std::vector<T> out;
      while (splitter.finished != true) {
        std::string_view original = splitter.getNext();
        T result;
        const auto [ptr, ec] = std::from_chars(original.data(), original.data() + original.size(), result);
        if (ec != std::errc())
          throw std::runtime_error(fmt::format("Fail to parse : {}", original));
        out.emplace_back(result);
      }
      return out;
    }

    Splitter splitter;
  };

  template <typename T, typename Splitter>
  struct ParseWithSplitter<T, Splitter, typename std::enable_if<std::is_integral_v<T>>::type> {
    ParseWithSplitter(Splitter splitter)
        : splitter(splitter) {
    }

    std::vector<T> apply() {
      std::vector<T> out;
      while (splitter.finished != true) {
        std::string_view original = splitter.getNext();
        T result;
        const auto [ptr, ec] = std::from_chars(original.data(), original.data() + original.size(), result);
        if (ec != std::errc())
          throw std::runtime_error(fmt::format("Fail to parse : {}", original));
        out.emplace_back(result);
      }
      return out;
    }

    Splitter splitter;
  };


  struct Splitter {
    Splitter(std::string_view input, char delimiter)
        : input(input), delimiter(delimiter) {
    }
    std::string_view getNext() {
      start = next;

      next = input.find(delimiter, start);

      if (next == std::string_view::npos) {
        finished = true;
        return std::string_view(input.data() + start);
      }

      // use next++ to increment after the string_view creation
      return std::string_view(input.data() + start, next++ - start);
    }

    size_t start = 0;
    size_t next = 0;
    bool finished = false;
    std::string_view input;
    const char delimiter;

    template <typename Container>
    operator Container() {
      return ParseWithSplitter<typename Container::value_type, Splitter>(*this).apply();
    }
  };
}

inline details::Splitter splitString(const std::string_view &input, char delimiter) {
  return details::Splitter(input, delimiter);
};

template <typename T>
inline T parse(std::string_view original, int base = 10) {
  T result;
  const auto [ptr, ec] = std::from_chars(original.data(), original.data() + original.size(), result, base);
  if (ec != std::errc())
    throw std::runtime_error(fmt::format("Fail to parse : {}", original));
  return result;
}

} // namespace aoc
