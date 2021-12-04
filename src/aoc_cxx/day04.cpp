#include "controller.hpp"

#include "aoc_rust.h"

#include <range/v3/all.hpp>
#include <spdlog/spdlog.h>

#include <charconv>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <tuple>
#include <vector>
#include <algorithm>

namespace aoc {

namespace fs = std::filesystem;
namespace rs = ranges;
namespace rv = ranges::views;

namespace {
inline std::vector<std::string> split(const std::string &original, char separator) {
  std::vector<std::string> results;
  auto start = original.begin();
  auto end = original.end();
  auto next = std::find(start, end, separator);

  while (next != end) {
    results.push_back(std::string(start, next));
    start = next + 1;
    next = std::find(start, end, separator);
  }

  results.push_back(std::string(start, next));

  return results;
}

template <typename T>
inline std::vector<T> parse(const std::vector<std::string> &input, int base = 10) {
  std::vector<T> result(input.size());
  for (size_t i = 0; i < input.size(); ++i) {
    const auto [ptr, ec] = std::from_chars(input[i].data(), input[i].data() + input[i].size(), result[i], base);
    if (ec != std::errc())
      throw std::runtime_error(fmt::format("Fail to parse : {}", input[i]));
  }
  return result;
}

template <int Dim>
std::array<uint64_t, Dim> countOnRow(const std::array<uint8_t, Dim*Dim> &mark) {
  std::array<uint64_t, Dim> counts{0};
  for (size_t row = 0; row < Dim; ++row) {
    for (size_t col = 0; col < Dim; ++col) {
      counts[row] += mark[row * Dim + col];
    }
  }
  return counts;
}

template <int Dim>
std::array<uint64_t, Dim> countOnColumn(const std::array<uint8_t, Dim*Dim> &mark) {
  std::array<uint64_t, Dim> counts{0};
  for (size_t row = 0; row < Dim; ++row) {
    for (size_t col = 0; col < Dim; ++col) {
      counts[col] += mark[row * Dim + col];
    }
  }
  return counts;
}

template <int Dim>
void updateBoard(const std::array<uint16_t, Dim*Dim> &board, std::array<uint8_t, Dim*Dim> &mark, uint16_t draw) {
  auto pos = board.begin();
  while (true) {
    pos = std::find(pos, board.end(), draw);
    if (pos == board.end())
      break;
    const size_t index = pos - board.begin();
    mark[index] = 1;
    ++pos;
  }
}

} // namespace

RegisterCommand day04("day04", {
    { "input_day04.txt",       38913,   16836},
    { "input_day04_test1.txt", 4512,    1924},
  }, [](fs::path filename) -> std::tuple<uint64_t, uint64_t> {

    std::ifstream infile(filename);
    if (!infile.is_open()) {
      throw std::runtime_error(fmt::format("File Not Found : {}", filename.string()));
    }
    constexpr int dim = 5;


    std::string line;

    getline(infile, line);
    std::vector<uint16_t> numbersDrawn = parse<uint16_t>(split(line, ','));

    std::vector<std::array<uint16_t, dim*dim>> boards;
    std::array<uint16_t, dim*dim> temporaryBoard{0};

    size_t lineInBoard = 0;
    while (getline(infile, line)) {
      if (line.empty())
        continue;

      std::istringstream instream(line);
      std::istream_iterator<uint16_t> start(instream), end;
      std::vector<uint16_t> boardLine(start, end);

      std::copy_n(boardLine.begin(), dim, temporaryBoard.begin()+lineInBoard);
      lineInBoard += dim;

      if (lineInBoard >= dim*dim) {
        boards.push_back(temporaryBoard);
        lineInBoard = 0;
      }
    }

    std::vector<std::array<uint8_t, dim*dim>> mark(boards.size(), {0});
    std::vector<uint8_t> boardsThatWin(boards.size(), 0);
    uint64_t firstWinScore = 0;
    uint64_t lastWinScore = 0;

    for (auto& draw : numbersDrawn) {
      spdlog::debug("draw:{}", draw);
      for (size_t i = 0; i < boards.size(); ++i) {
        // skip board that already win
        if (boardsThatWin[i] == 1)
          continue;
        updateBoard<dim>(boards[i], mark[i], draw);

        bool hasCompleted = false;
        const std::array<uint64_t, dim> colCount = countOnColumn<dim>(mark[i]);
        for (auto& j : colCount) {
          if (j == dim)
            hasCompleted = true;
        }
        const std::array<uint64_t, dim> rowCount = countOnRow<dim>(mark[i]);
        for (auto& j : rowCount) {
          if (j == dim)
            hasCompleted = true;
        }

        if (hasCompleted) {
          boardsThatWin[i] = 1;
          uint64_t sumMarked = 0;
          for (size_t index = 0; index < boards[i].size(); ++index) {
            if (mark[i][index] != 1) {
              sumMarked += boards[i][index];
            }
            spdlog::debug("board={} mark={}", boards[i][index], mark[i][index]);
          }
          if (firstWinScore == 0)
            firstWinScore = sumMarked * draw;
          lastWinScore = sumMarked * draw;
        }

        if (std::accumulate(boardsThatWin.begin(), boardsThatWin.end(), 0) == boards.size())
          return {firstWinScore, lastWinScore};
      }
    }

    return {0, 0};
});


} // namespace aoc