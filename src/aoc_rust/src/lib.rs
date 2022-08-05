mod day01;
mod day02;
mod day03;
mod day13;
mod day14;
mod day15;

use crate::day01::day01;
use crate::day01::day01functional;
use crate::day02::day02functional;
use crate::day03::day03functional;
use crate::day13::day13;
use crate::day14::day14;
use crate::day15::day15;

#[cxx::bridge(namespace = "aoc::rust")]
mod ffi {
  extern "Rust" {

    fn day01(filename: &CxxString) -> [u64; 2];
    fn day01functional(filename: &CxxString) -> [u64; 2];
    fn day02functional(filename: &CxxString) -> [u64; 2];
    fn day03functional(filename: &CxxString) -> [u64; 2];
    fn day13(filename: &CxxString) -> [u64; 2];
    fn day14(filename: &CxxString) -> [u64; 2];
    fn day15(filename: &CxxString) -> [u64; 2];

  }
}
