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
