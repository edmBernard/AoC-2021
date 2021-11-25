use cxx::CxxString;

// #[cxx::bridge(namespace = "aoc::rust")]
// mod ffi {
//   extern "Rust" {

//     fn day01(filename: &CxxString) -> u16;
//   }
// }

pub fn day01(filename: &CxxString) -> u16 {
  println!("Message from rust");
  println!("{}", filename);
  42
}
