mod day01;
mod day02;
mod day03;

use crate::day01::day01;
use crate::day01::day01functional;
use crate::day02::day02functional;
use crate::day03::day03functional;

#[cxx::bridge(namespace = "aoc::rust")]
mod ffi {
  extern "Rust" {

    // it's an ugly signature but cxx_rust don't support tuple yet.
    // so I can't return result as a tuple that will be use in c++
    fn day01(filename: &CxxString) -> [u64; 2];
    fn day01functional(filename: &CxxString) -> [u64; 2];
    fn day02functional(filename: &CxxString) -> [u64; 2];
    fn day03functional(filename: &CxxString) -> [u64; 2];
  }
}
