use cxx::CxxString;

// #[cxx::bridge(namespace = "aoc::rust")]
// mod ffi {
//   extern "Rust" {

//     fn day02(filename: &CxxString) -> u16;
//   }
// }

pub fn day02(filename: &CxxString) -> u16 {
  println!("Message from rust");
  println!("{}", filename);
  42
}
