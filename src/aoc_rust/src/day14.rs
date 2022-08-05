use cxx::CxxString;

use std::fs;
use std::convert::TryInto;
use std::collections::HashMap;

pub fn day14(filename: &CxxString) -> [u64; 2] {

  let input_puzzle = fs::read_to_string(filename.to_str().unwrap()).expect("File not found!");
  let lines: Vec<&str> = input_puzzle.lines().collect();

  let starting_state : String = String::from(lines[0]);

  let mut substitution_rules = HashMap::new();

  for line in &lines[1..] {

    if line.trim().len() == 0 {
      continue;
    }
    let [key, value] : [&str; 2] = line.split(" -> ").collect::<Vec<_>>().try_into().expect("Don't have proper rule expect XY -> Z");

    substitution_rules.insert(key, value);
  }

  let mut state = starting_state;

  for _step in 0..10 {

    state = state.chars()
                 .collect::<Vec<char>>()
                 .windows(2)
                 .map(|value| vec![value[0].to_string(), substitution_rules.get(&*value.iter().collect::<String>()).unwrap().to_string()] )
                 .flat_map(|s| s.into_iter())
                 .collect::<String>() + &state.chars().last().unwrap().to_string();
  }

  let mut histogram = HashMap::new();
  state.chars().for_each(|key| if histogram.contains_key(&key) { *histogram.get_mut(&key).unwrap() += 1; } else { histogram.insert(key, 1); });

  let most_common = histogram.values().max().unwrap();
  let least_common = histogram.values().min().unwrap();

  for (k, v) in histogram.iter() {
    println!("{} -> {}", k, v);
  }

  [most_common - least_common, 1]
}
