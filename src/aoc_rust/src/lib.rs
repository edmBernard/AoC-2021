mod day01;
mod day02;

use crate::day01::day01;
use crate::day01::day01raw;
use crate::day02::day02;

#[cxx::bridge(namespace = "aoc::rust")]
mod ffi {
  extern "Rust" {

    // it's an ugly signature but cxx_rust don't support tuple yet.
    // so I can't return result as a tuple that will be use in c++
    fn day01(filename: &CxxString, part1: &mut u64, part2: &mut u64);
    fn day01raw(filename: &CxxString, part1: &mut u64, part2: &mut u64);
    fn day02(filename: &CxxString, part1: &mut u64, part2: &mut u64);
  }
}
