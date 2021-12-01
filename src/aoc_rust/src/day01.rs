use cxx::CxxString;

use std::fs::File;
use std::io::{BufReader, BufRead};

pub fn day01(filename: &CxxString, part1: &mut u64, part2: &mut u64) {

  let input = File::open(filename.to_str().unwrap()).expect("File not found!");
  let buffered = BufReader::new(input);

  let mut input_puzzle = Vec::new();
  for line in buffered.lines() {
    let value : u32 = line.unwrap().parse().unwrap();
    input_puzzle.push(value);
  }

  // part1
  let mut previous = input_puzzle[0];
  for &depth in &input_puzzle {
    if depth > previous {
      *part1 += 1
    }
    previous = depth;
  }

  // part2
  let mut previous = input_puzzle[0] +  input_puzzle[1] +  input_puzzle[2];
  for idx in 2..input_puzzle.len() {
    let sum = input_puzzle[idx] +  input_puzzle[idx-1] +  input_puzzle[idx-2];
    if sum > previous {
      *part2 += 1
    }
    previous = sum;
  }
}

pub fn day01raw(filename: &CxxString, part1: &mut u64, part2: &mut u64) {

  let input_puzzle: Vec<u16> = std::fs::read_to_string(filename.to_str().unwrap())
    .expect("File not found!")
    .trim()
    .split("\n")
    .map(|x| x.parse().unwrap())
    .collect();

  // part1
  let mut previous = input_puzzle[0];
  for &depth in &input_puzzle {
    if depth > previous {
      *part1 += 1
    }
    previous = depth;
  }

  // part2
  let mut previous = input_puzzle[0] +  input_puzzle[1] +  input_puzzle[2];
  for idx in 2..input_puzzle.len() {
    let sum = input_puzzle[idx] +  input_puzzle[idx-1] +  input_puzzle[idx-2];
    if sum > previous {
      *part2 += 1
    }
    previous = sum;
  }
}
