use cxx::CxxString;

use std::io::{BufReader, BufRead};

pub fn day01(filename: &CxxString) -> [u64; 2] {

  let input = std::fs::File::open(filename.to_str().unwrap()).expect("File not found!");
  let buffered = BufReader::new(input);

  let mut input_puzzle = Vec::new();
  for line in buffered.lines() {
    let value : u16 = line.unwrap().parse().unwrap();
    input_puzzle.push(value);
  }

  // part1
  let mut part1 : u64 = 0;
  let mut previous = input_puzzle[0];
  for &depth in &input_puzzle {
    if depth > previous {
      part1 += 1
    }
    previous = depth;
  }

  // part2
  let mut part2 : u64 = 0;
  let mut previous = input_puzzle[0] +  input_puzzle[1] +  input_puzzle[2];
  for idx in 2..input_puzzle.len() {
    let sum = input_puzzle[idx] +  input_puzzle[idx-1] +  input_puzzle[idx-2];
    if sum > previous {
      part2 += 1
    }
    previous = sum;
  }

  [part1, part2]
}

pub fn day01functional(filename: &CxxString) -> [u64; 2] {

  let input_puzzle = std::fs::read_to_string(filename.to_str().unwrap())
    .expect("File not found!")
    .lines()
    .map(|x| x.parse::<u16>().unwrap())
    .collect::<Vec<_>>();

  // part1
  let part1 : u64 = input_puzzle.windows(2)
  .filter(|pair| pair[0] < pair[1])
  .count() as u64;

  // part2
  let part2 : u64 = input_puzzle
    .windows(3)
    .map(|triplet| triplet.into_iter().sum())
    .collect::<Vec<u16>>()
    .windows(2)
    .filter(|pair| pair[0] < pair[1])
    .count() as u64;

  [part1, part2]
}
