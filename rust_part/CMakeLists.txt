if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CARGO_CMD cargo build --verbose)
    set(TARGET_DIR "Debug")
    message("In DEBUG")
else ()
    set(CARGO_CMD cargo build --release --verbose)
    set(TARGET_DIR "Release")
    message("In RELEASE")
endif ()
message("CMAKE_BUILD_TYPE" ${CMAKE_BUILD_TYPE})
message("CMAKE_CONFIGURATION_TYPES" ${CMAKE_CONFIGURATION_TYPES})

set(RUST_PART_LIB "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_DIR}/rust_part.lib")

set(RUST_PART_CXX "${CMAKE_CURRENT_BINARY_DIR}/rust_part.cpp")
add_library(rust_part_cxx STATIC ${RUST_PART_CXX})
add_custom_command(
    OUTPUT ${RUST_PART_CXX}
    COMMAND ${CARGO_CMD} --target-dir=${CMAKE_CURRENT_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/cxxbridge/rust_part/src/lib.rs.cc ${RUST_PART_CXX}
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/cxxbridge/rust_part/src/lib.rs.h ${CMAKE_CURRENT_BINARY_DIR}/rust_part.h
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
  # Library for rustup : https://github.com/rust-lang/rustup/pull/1468
  set(EXTRA_LINK_LIBRARY userenv advapi32 ws2_32 shell32 msvcrt)
else()
  set(EXTRA_LINK_LIBRARY pthread dl)
endif()

message("RUST_PART_LIB" ${RUST_PART_LIB})
target_link_libraries(rust_part_cxx ${EXTRA_LINK_LIBRARY} ${RUST_PART_LIB})
target_include_directories(rust_part_cxx PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME rust_part_test
    COMMAND cargo test
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

message("##################################")