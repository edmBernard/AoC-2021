#[cxx::bridge]
mod ffi {
  extern "Rust" {

    fn addition(a: u16, b: u16) -> u16;
  }
}

pub fn addition(a: u16, b: u16) -> u16 {
  a + b
}
