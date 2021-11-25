
mod day01;
mod day02;

use crate::day01::day01;
use crate::day02::day02;

#[cxx::bridge(namespace = "aoc::rust")]
mod ffi {
  extern "Rust" {

    fn day02(filename: &CxxString) -> u16;
    fn day01(filename: &CxxString) -> u16;

  }
}

// pub fn day02(filename: &CxxString) -> u16 {
//   println!("Message from rust");
//   println!("{}", filename);
//   day01::day01(filename)
// }
