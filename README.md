# AoC-2021

- Github : [https://github.com/edmBernard/AoC-2021](https://github.com/edmBernard/AoC-2021)

## Dependencies

We use [vcpkg](https://github.com/Microsoft/vcpkg) to manage dependencies

This project depends on:
- [cxxopts](https://github.com/jarro2783/cxxopts): Command line argument parsing
- [fmt](https://fmt.dev/latest/index.html): A modern formatting library
- [spdlog](https://github.com/gabime/spdlog): Very fast, header-only/compiled, C++ logging library
- [xsimd](https://github.com/xtensor-stack/xsimd): C++ wrappers for SIMD intrinsics and parallelized, optimized mathematical functions
- [range-v3](https://ericniebler.github.io/range-v3/): Range library for C++14/17/20


```
./vcpkg install spdlog cxxopts fmt xsimd range-v3
```

### Compilation

```bash
mkdir build
cd build
# configure make with vcpkg toolchain
cmake .. -DCMAKE_TOOLCHAIN_FILE=${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake
# on Windows : cmake .. -DCMAKE_TOOLCHAIN_FILE=${env:VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release
```

The executable is named `AoC-2021`

## Disclaimer

It's a toy project. So if you spot error, improvement comments are welcome.


## Problems

On my `Intel(R) Core(TM) i5-4570 CPU @ 3.20GHz`

```bash
[2021-12-10 08:32:20.241] [info] day01                     in    0.06 ms : part1=1233       part2=1275
[2021-12-10 08:32:20.339] [info] day01,simple              in    0.09 ms : part1=1233       part2=1275
[2021-12-10 08:32:20.440] [info] day01,range               in    0.10 ms : part1=1233       part2=1275
[2021-12-10 08:32:20.557] [info] day01,rust                in    0.11 ms : part1=1233       part2=1275
[2021-12-10 08:32:20.607] [info] day01,rust,functional     in    0.05 ms : part1=1233       part2=1275
[2021-12-10 08:32:20.656] [info] day02                     in    0.04 ms : part1=2117664    part2=2073416724
[2021-12-10 08:32:20.820] [info] day02,range               in    0.16 ms : part1=2117664    part2=2073416724
[2021-12-10 08:32:20.947] [info] day02,rust,functional     in    0.12 ms : part1=2117664    part2=2073416724
[2021-12-10 08:32:21.073] [info] day03                     in    0.12 ms : part1=3429254    part2=5410338
[2021-12-10 08:32:21.130] [info] day03,rust,functional     in    0.05 ms : part1=3429254    part2=5410338
[2021-12-10 08:32:21.490] [info] day04                     in    0.35 ms : part1=38913      part2=16836
[2021-12-10 08:32:22.217] [info] day05                     in    0.71 ms : part1=7380       part2=21373
[2021-12-10 08:32:22.282] [info] day06                     in    0.03 ms : part1=345793     part2=1572643095893
[2021-12-10 08:32:22.334] [info] day07                     in    0.03 ms : part1=340052     part2=92948968
[2021-12-10 08:32:22.952] [info] day08                     in    0.61 ms : part1=495        part2=1055164
[2021-12-10 08:32:23.260] [info] day09                     in    0.28 ms : part1=564        part2=1038240
[2021-12-10 08:32:23.356] [info] day10                     in    0.09 ms : part1=243939     part2=2421222841
```

| Day | Description | Tips |
|--|--|--|
| Day01 | Edge detection | - |
| Day02 | Move submarine | - |
| Day03 | Binary tree | Use tree descent |
| Day04 | Bingo | - |
| Day05 | Draw Line | - |
| Day06 | Fish population | Memoization to precompute fish multiplication |
| Day07 | Crab best position | Dichotomy to find the minimum position |
| Day08 | Find digit | - |
| Day09 | Basin size | Watershed-like algorithm |
| Day10 | Bracket matching | Use a stack |
