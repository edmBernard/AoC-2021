cmake_minimum_required(VERSION 3.21)

#**************************************************************************************************
# Project *****************************************************************************************
project(AoC-2021
        HOMEPAGE_URL https://github.com/edmBernard/AoC-2021)

#**************************************************************************************************
# Find Package ************************************************************************************
find_package(cxxopts REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

find_package(xsimd CONFIG REQUIRED)
find_package(range-v3 CONFIG REQUIRED)

#**************************************************************************************************
# Include *****************************************************************************************
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()

#**************************************************************************************************
# Set compiler ************************************************************************************
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
  add_compile_options(/Oy /O2 /utf-8 /Zc:__cplusplus /DUNICODE /D_UNICODE /arch:AVX2 /MD)
else()
  # add_compile_options(-fPIC -mavx2 -fno-omit-frame-pointer -g)
  add_compile_options(-O3 -mavx2)
endif()

#**************************************************************************************************
# Set variable ************************************************************************************
file(GLOB COMMANDS_FILE ${CMAKE_CURRENT_SOURCE_DIR}/src/aoc_cxx/*.cpp)

add_subdirectory(src/aoc_rust)

SET(SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)
#**************************************************************************************************
# Make configuration ******************************************************************************
add_executable(AoC-2021 ${COMMANDS_FILE} ${SOURCES})
target_link_libraries(AoC-2021
  PRIVATE fmt::fmt-header-only spdlog::spdlog_header_only cxxopts::cxxopts
  xsimd
  range-v3 range-v3-meta range-v3::meta range-v3-concepts
  aoc::rust
)
