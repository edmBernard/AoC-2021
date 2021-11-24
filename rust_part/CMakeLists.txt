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

message("CMAKE_BUILD_TYPE" ${CMAKE_BUILD_TYPE})
message("CMAKE_CONFIGURATION_TYPES" ${CMAKE_CONFIGURATION_TYPES})

set(RUST_LIB "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_DIR}/rust_part.lib")

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/rust_part.cpp
  COMMAND ${CARGO_CMD} --target-dir=${CMAKE_CURRENT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/cxxbridge/rust_part/src/lib.rs.cc ${CMAKE_CURRENT_BINARY_DIR}/rust_part.cpp
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/cxxbridge/rust_part/src/lib.rs.h ${CMAKE_CURRENT_BINARY_DIR}/rust_part.h
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(rust_part_cxx STATIC ${CMAKE_CURRENT_BINARY_DIR}/rust_part.cpp)

if (MSVC)
  # Library for rustup : https://github.com/rust-lang/rustup/pull/1468
  set(EXTRA_LINK_LIBRARY userenv advapi32 ws2_32 shell32 msvcrt)
  # set(EXTRA_LINK_LIBRARY userenv advapi32 ws2_32 shell32 $<IF:$<OR:$<CONFIG:Debug>,$<CONFIG:>>,msvcrtd,msvcrt>)
else()
  set(EXTRA_LINK_LIBRARY pthread m dl)
endif()

target_link_libraries(rust_part_cxx ${EXTRA_LINK_LIBRARY} ${RUST_LIB})
target_include_directories(rust_part_cxx PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME rust_part_test
    COMMAND cargo test
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
