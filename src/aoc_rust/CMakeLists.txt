if (NOT CMAKE_CONFIGURATION_TYPES)
  message("Single configuration generator")
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CARGO_CMD cargo build --verbose)
    set(TARGET_DIR "debug")
    message("In DEBUG")
  else ()
    set(CARGO_CMD cargo build --release --verbose)
    set(TARGET_DIR "release")
    message("In RELEASE")
  endif ()
else()
  message("Multi-configuration generator")
  set(CARGO_CMD cargo build $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:>>>:--release> --verbose)
  set(TARGET_DIR $<IF:$<OR:$<CONFIG:Debug>,$<CONFIG:>>,debug,release>)
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(LIBRARY_EXTENSION "lib")
  set(LIBRARY_PREFIX "")
else()
  set(LIBRARY_EXTENSION "a")
  set(LIBRARY_PREFIX "lib")
endif()

# the rust lib that only contain rust code
set(RUST_LIB "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_DIR}/${LIBRARY_PREFIX}aoc_rust.${LIBRARY_EXTENSION}")

add_custom_command(
  OUTPUT ${RUST_LIB} ${CMAKE_CURRENT_BINARY_DIR}/aoc_rust.cpp
  COMMAND ${CARGO_CMD} --target-dir=${CMAKE_CURRENT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/cxxbridge/aoc_rust/src/lib.rs.cc ${CMAKE_CURRENT_BINARY_DIR}/aoc_rust.cpp
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/cxxbridge/aoc_rust/src/lib.rs.h ${CMAKE_CURRENT_BINARY_DIR}/aoc_rust.h
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  USES_TERMINAL
)

# the c++ library that encapsulate the rust library
add_library(aoc_rust_cxx STATIC ${CMAKE_CURRENT_BINARY_DIR}/aoc_rust.cpp)

if (MSVC)
  # Library for rustup : https://github.com/rust-lang/rustup/pull/1468
  set(EXTRA_LINK_LIBRARY userenv advapi32 ws2_32 shell32 msvcrt)
  # it seem msvcrt don't need the debug version even in debug
  # set(EXTRA_LINK_LIBRARY userenv advapi32 ws2_32 shell32 $<IF:$<OR:$<CONFIG:Debug>,$<CONFIG:>>,msvcrtd,msvcrt>)
else()
  set(EXTRA_LINK_LIBRARY -Wl,--no-as-needed pthread m dl)
endif()

target_link_libraries(aoc_rust_cxx ${EXTRA_LINK_LIBRARY} ${RUST_LIB})
target_include_directories(aoc_rust_cxx PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME aoc_rust_test
    COMMAND cargo test
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_library(aoc::rust ALIAS aoc_rust_cxx)
