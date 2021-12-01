use cxx::CxxString;

use std::fs::File;
use std::io::{BufReader, BufRead};

pub fn day01(filename: &CxxString, part1: &mut u64, part2: &mut u64) {

  let input = File::open(filename.to_str().unwrap()).expect("File not found!");
  let buffered = BufReader::new(input);

  let mut depth_list = Vec::new();
  for line in buffered.lines() {
    let value : u32 = line.unwrap().parse().unwrap();
    depth_list.push(value);
  }

  // part1
  let mut previous = &depth_list[0];
  for depth in &depth_list {
    *part1 += if depth > previous {
      1
    } else {
      0
    };
    previous = depth;
  }

  // part2
  let mut previous = depth_list[0] +  depth_list[1] +  depth_list[2];
  for idx in 0..depth_list.len()-2 {
    let sum = depth_list[idx] +  depth_list[idx+1] +  depth_list[idx+2];
    *part2 += if sum > previous {
      1
    } else {
      0
    };
    previous = sum;
  }
}

pub fn day01raw(filename: &CxxString, part1: &mut u64, part2: &mut u64) {

  let input_puzzle: Vec<u32> = std::fs::read_to_string(filename.to_str().unwrap())
    .expect("File not found!")
    .trim()
    .split("\n")
    .map(|x| x.parse().unwrap())
    .collect();

  // part1
  let mut previous = &input_puzzle[0];
  for depth in &input_puzzle {
    *part1 += if depth > previous {
      1
    } else {
      0
    };
    previous = depth;
  }

  // part2
  let mut previous = input_puzzle[0] +  input_puzzle[1] +  input_puzzle[2];
  for idx in 0..input_puzzle.len()-2 {
    let sum = input_puzzle[idx] +  input_puzzle[idx+1] +  input_puzzle[idx+2];
    *part2 += if sum > previous {
      1
    } else {
      0
    };
    previous = sum;
  }
}
