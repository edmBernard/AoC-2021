use cxx::CxxString;

#[cxx::bridge]
mod ffi {
  extern "Rust" {

    fn addition(a: u16, b: u16, key: &CxxString) -> u16;
  }
}

pub fn addition(a: u16, b: u16, key: &CxxString) -> u16 {
  println!("Message from rust");
  println!("{}", key);
  a + b
}
