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
#include <numeric>

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


template <int Dim, typename T>
void showBoard(const std::array<T, Dim*Dim> &mark) {
  for (size_t row = 0; row < Dim; ++row) {
    for (size_t col = 0; col < Dim; ++col) {
      fmt::print("{} ", mark[row * Dim + col]);
    }
    fmt::print("\n");
  }
}


template <int Dim, typename T>
void showVect(const std::array<T, Dim> &mark) {
  for (size_t col = 0; col < Dim; ++col) {
    fmt::print("{} ", mark[col]);
  }
  fmt::print("\n");
}


template <int Dim>
bool detectWin(const std::array<bool, Dim*Dim> &mark) {
  std::array<bool, Dim> countsRow;
  std::array<bool, Dim> countsCol;
  countsRow.fill(true);
  countsCol.fill(true);
  for (size_t row = 0; row < Dim; ++row) {
    for (size_t col = 0; col < Dim; ++col) {
      const bool value = mark[row * Dim + col];
      countsCol[col] &= value;
      countsRow[row] &= value;
      // check if win on last row only
      if (row == Dim - 1 && countsCol[col])
        return true;
    }
    // check if win on last col
    if (countsRow[row])
      return true;
  }

  return false;
}

template <int Dim>
bool updateBoard(const std::array<uint16_t, Dim*Dim> &board,
                 std::array<bool, Dim*Dim> &mark,
                 std::array<uint8_t, Dim>& countColumns,
                 std::array<uint8_t, Dim>& countRows,
                 uint16_t draw) {
  for (size_t row = 0; row < Dim; ++row) {
    for (size_t col = 0; col < Dim; ++col) {
      if (board[row * Dim + col] == draw) {
        mark[row * Dim + col] = true;
        // increment columns count before check winner
        return ++countRows[row] == Dim || ++countColumns[col] == Dim;
      }
    }
  }
  return false;
}

template <int Dim>
uint64_t getSum(const std::array<uint16_t, Dim*Dim> &board, std::array<bool, Dim*Dim> &mark, uint16_t draw) {
  uint64_t sumMarked = 0;
  for (size_t index = 0; index < board.size(); ++index) {
    if (mark[index] != 1) {
      sumMarked += board[index];
    }
  }
  return sumMarked * draw;
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

    std::vector<std::array<bool, dim*dim>> mark(boards.size(), {0});
    std::vector<std::array<uint8_t, dim>> countColumns(boards.size(), {0});
    std::vector<std::array<uint8_t, dim>> countRows(boards.size(), {0});
    std::vector<uint8_t> boardsThatWin(boards.size(), 0);
    uint64_t firstWinScore = 0;
    uint64_t lastWinScore = 0;

    for (auto& draw : numbersDrawn) {
      for (size_t i = 0; i < boards.size(); ++i) {
        // skip board that already win
        if (boardsThatWin[i] == 1)
          continue;

        // update mark in function of the drawn number
        const bool hasCompleted = updateBoard<dim>(boards[i], mark[i], countColumns[i], countRows[i], draw);

        // if the board win compute the score
        if (hasCompleted) {
          boardsThatWin[i] = 1;

          if (firstWinScore == 0) {
            firstWinScore = getSum<dim>(boards[i], mark[i], draw);
          } else if (std::accumulate(boardsThatWin.begin(), boardsThatWin.end(), 0) == boards.size()) {
            // check if it's the last winner we can't be first and last winner
            return {firstWinScore, getSum<dim>(boards[i], mark[i], draw)};
          }
        }
      }
    }

    return {0, 0};
});


} // namespace aoc
