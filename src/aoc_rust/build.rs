fn main () {
  let _build = cxx_build::bridge("src/lib.rs")
                          .file("src/day01.rs")
                          .file("src/day02.rs")
                          .file("src/day03.rs")
                          .file("src/day13.rs")
                          .file("src/day14.rs")
                          ;
  println!("cargo:rerun-if-changed=src/lib.rs");
}
