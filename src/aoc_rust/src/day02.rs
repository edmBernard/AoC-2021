use cxx::CxxString;

pub fn day02(filename: &CxxString, part1: &mut u64, part2: &mut u64) {
  *part1 = 42;
  *part2 = 43;
  println!("Message from rust");
  println!("{}", filename);
}
