use cxx::CxxString;

fn get_rule(input: &[u32], index: u32) -> u32 {
  let counts = input.iter().filter(|x| (x.clone() >> index) & 1 != 0).count() as u32;
  if counts >= input.len() as u32 - counts { 1 } else { 0 }
}

fn tree_decent(input: &[u32], line_length : u32, option : bool) -> u32 {
  let mut input_copy = input.to_vec();
  for i in (0..line_length).rev() {
    let rule = get_rule(&input_copy, i as u32) ^ option as u32;
    input_copy.retain(|x| (x >> i) & 1 == rule);
    if input_copy.len() == 1 {
      break;
    }
  }
  input_copy[0]
}

pub fn day03functional(filename: &CxxString) -> [u64; 2] {

  let mut line_length = 0;

  let input_puzzle = std::fs::read_to_string(filename.to_str().unwrap())
    .expect("File not found!")
    .lines()
    .map(|x| {
      // I don't find a proper to do that in "rust"
      if line_length == 0 {
        line_length = x.chars().count() as u32
      };
      x
    })
    .map(|x| u32::from_str_radix(x, 2).unwrap())
    .collect::<Vec<_>>();

  let mut gamma = 0;
  for i in 0..line_length as u32 {
    gamma += get_rule(&input_puzzle, i) << i;
  }
  let epsilon = (1 << line_length) - 1 & !gamma;

  let part1 = (gamma * epsilon) as u64;

  let oxygen = tree_decent(&input_puzzle, line_length, false);
  let co2 = tree_decent(&input_puzzle, line_length, true);

  let part2 = (oxygen * co2) as u64;

  [part1, part2]
}