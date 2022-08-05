mod day01;
mod day02;
mod day03;
mod day13;

use crate::day01::day01;
use crate::day01::day01functional;
use crate::day02::day02functional;
use crate::day03::day03functional;
use crate::day13::day13;

#[cxx::bridge(namespace = "aoc::rust")]
mod ffi {
  extern "Rust" {

    fn day01(filename: &CxxString) -> [u64; 2];
    fn day01functional(filename: &CxxString) -> [u64; 2];
    fn day02functional(filename: &CxxString) -> [u64; 2];
    fn day03functional(filename: &CxxString) -> [u64; 2];
    fn day13(filename: &CxxString) -> [u64; 2];


  }
}
