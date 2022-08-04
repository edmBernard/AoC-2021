set(CARGO_CMD cargo build $<$<CONFIG:release>:--release> --verbose)
set(TARGET_DIR $<IF:$<CONFIG:release>,release,debug>)

message(STATUS "Library prefix used for rust generated library : ${CMAKE_FIND_LIBRARY_PREFIXES}")
message(STATUS "Library suffix used for rust generated library : ${CMAKE_STATIC_LIBRARY_SUFFIX}")

# the rust lib that only contain rust code
set(RUST_LIB "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_DIR}/${CMAKE_FIND_LIBRARY_PREFIXES}aoc_rust${CMAKE_STATIC_LIBRARY_SUFFIX}")

# list of rust file to trigger cargo on change
file(GLOB RUST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.rs)

# use a custom target instead of a custom command to force the regeneration
add_custom_command(
  OUTPUT  ${RUST_LIB} ${CMAKE_CURRENT_BINARY_DIR}/aoc_rust.cpp ${CMAKE_CURRENT_BINARY_DIR}/aoc_rust.h
  COMMAND ${CARGO_CMD} --target-dir=${CMAKE_CURRENT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E echo "Target dir : " ${TARGET_DIR}
  COMMAND ${CMAKE_COMMAND} -E echo "Cargo command : " ${CARGO_CMD}
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/cxxbridge/aoc_rust/src/lib.rs.cc ${CMAKE_CURRENT_BINARY_DIR}/aoc_rust.cpp
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/cxxbridge/aoc_rust/src/lib.rs.h ${CMAKE_CURRENT_BINARY_DIR}/aoc_rust.h
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS ${RUST_SOURCES}
  USES_TERMINAL
)

# the c++ library that encapsulate the rust library
add_library(aoc_rust_cxx STATIC ${CMAKE_CURRENT_BINARY_DIR}/aoc_rust.cpp)

if (MSVC)
  # Library for rustup : https://github.com/rust-lang/rustup/pull/1468
  set(EXTRA_LINK_LIBRARY userenv advapi32 ws2_32 shell32 msvcrt)
  # it seem msvcrt don't need the debug version even in debug
  # set(EXTRA_LINK_LIBRARY userenv advapi32 ws2_32 shell32 $<IF:$<OR:$<CONFIG:Debug>,$<CONFIG:>>,msvcrtd,msvcrt>)
elseif (APPLE)
  set(EXTRA_LINK_LIBRARY pthread m dl)
else ()
  # -Wl,--no-as-needed use to prevent missing unused import like dlsym
  set(EXTRA_LINK_LIBRARY -Wl,--no-as-needed pthread m dl)
endif()

target_link_libraries(aoc_rust_cxx ${EXTRA_LINK_LIBRARY} ${RUST_LIB})
target_include_directories(aoc_rust_cxx PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME aoc_rust_test
    COMMAND cargo test
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_library(aoc::rust ALIAS aoc_rust_cxx)
