use cxx::CxxString;

use std::fs;

fn fill_count_map(weight: &Vec<u8>, accumulator: &mut Vec<u64>, current: u64, pos: [i32; 2], size: [i32; 2]) -> u64 {
  let [x, y] = pos;
  let offset = (x + y * size[0]) as usize;

  let next_weight = |x, y| current + weight[(x + y * size[0]) as usize] as u64;

  if accumulator[offset] == 0 || current < accumulator[offset] {
    accumulator[offset] = current;
    if offset == weight.len()-1 {
      return accumulator[offset];
    }
    let results = vec![fill_count_map(weight, accumulator, next_weight((x + 1).clamp(0, size[0]-1), y), [(x + 1).clamp(0, size[0]-1), y], size),
                       fill_count_map(weight, accumulator, next_weight((x - 1).clamp(0, size[0]-1), y), [(x - 1).clamp(0, size[0]-1), y], size),
                       fill_count_map(weight, accumulator, next_weight(x, (y + 1).clamp(0, size[1]-1)), [x, (y + 1).clamp(0, size[1]-1)], size),
                       fill_count_map(weight, accumulator, next_weight(x, (y - 1).clamp(0, size[1]-1)), [x, (y - 1).clamp(0, size[1]-1)], size)
                      ];
    *results.iter().filter(|&value| *value != 0u64).min().unwrap_or(&0u64)
  } else {
    0
  }
}

pub fn day15(filename: &CxxString) -> [u64; 2] {

  let mut line_length: usize = 0;
  let input_puzzle = fs::read_to_string(filename.to_str().unwrap()).expect("File not found!")
                          .lines()
                          .flat_map(|l| {
                            // I don't find a proper to do that in "rust"
                            if line_length == 0 {
                              line_length = l.chars().count()
                            };
                            l.chars()
                            .map(|c| c.to_digit(10).unwrap() as u8)
                            .collect::<Vec<_>>().into_iter()
                          })
                          .collect::<Vec<_>>();

  let mut accumulator: Vec<u64> = vec![0; input_puzzle.len()];

  let nrows = (input_puzzle.len() / line_length) as i32;
  let part1 = fill_count_map(&input_puzzle, &mut accumulator, 0, [0, 0], [line_length as i32, nrows]);

  [part1, 1]
}
