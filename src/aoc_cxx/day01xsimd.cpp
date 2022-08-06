#include "controller.hpp"

#include <spdlog/spdlog.h>
#include <xsimd/xsimd.hpp>

#include <filesystem>
#include <fstream>
#include <vector>

namespace aoc {

namespace xs = xsimd;
namespace fs = std::filesystem;

const RegisterCommand day01xsimd("day01,xsimd", {{"day01.txt", 1, 2}}, [](fs::path filename) -> std::array<uint64_t, 2> {
  spdlog::debug("filename: {}", filename.string());

  std::ifstream infile(filename);
  if (!infile.is_open()) {
    throw std::runtime_error(fmt::format("File Not Found : {}", filename.string()));
  }

  std::vector<int32_t> numberList;
  std::string line;
  while (infile >> line) {
    numberList.push_back(std::stoi(line));
  }
  std::vector<int32_t> res(numberList.size(), 0);

  // for (auto& v : numberList) {
  //   fmt::print("{}, ", v);
  // }
  // for (auto& v : res) {
  //   fmt::print("{}, ", v);
  // }
  using b_type = xs::batch<int32_t, xs::avx>;
  std::size_t inc = b_type::size;
  std::size_t size = res.size();
  // fmt::print("\ninc: {}, \n", inc);

  // size for which the vectorization is possible
  std::size_t vec_size = size - size % inc;
  for(std::size_t i = 0; i < vec_size; i +=inc)
  {
      b_type avec = b_type::load_unaligned(&numberList[i]);
      b_type rvec = avec * 2;
      rvec.store_unaligned(&res[i]);
  }
  // Remaining part that cannot be vectorize
  for(std::size_t i = vec_size; i < size; ++i)
  {
      res[i] = (numberList[i] * 2);
  }

  // for (auto& v : res) {
  //   fmt::print("{}, ", v);
  // }
  return {1, 2};
});

} // namespace aoc