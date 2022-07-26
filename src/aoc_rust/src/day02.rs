use cxx::CxxString;

enum Direction {
  Forward,
  Up,
  Down,
}

fn parse_direction(direction : &str) -> Direction {
  match direction {
    "forward" => Direction::Forward,
    "up" => Direction::Up,
    "down" => Direction::Down,
    _ => panic!("Unknown direction")
  }
}

pub fn day02functional(filename: &CxxString) -> [u64; 2] {
  let input_puzzle = std::fs::read_to_string(filename.to_str().unwrap())
    .expect("File not found!")
    .lines()
    .map(|line| line.split(' ').collect::<Vec<_>>())
    .map(|line_splitted| (parse_direction(line_splitted[0]), line_splitted[1].parse::<u64>().unwrap()))
    .collect::<Vec<_>>();

  let mut position : u64 = 0;
  let mut depth_part1 : u64 = 0;
  let mut depth_part2 : u64 = 0;
  let mut aim : u64 = 0;

  for (dir, value) in input_puzzle {
    match dir {
      Direction::Forward => {
        position += value;
        depth_part2 += value * aim;
      },
      Direction::Up => {
        depth_part1 -= value;
        aim -= value;
      },
      Direction::Down => {
        depth_part1 += value;
        aim += value;
      },
    }
  }

  [depth_part1 * position, depth_part2 * position]
}
