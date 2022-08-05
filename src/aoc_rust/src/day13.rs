use cxx::CxxString;

use std::fs;
use std::convert::TryInto;

fn fold(sheet: Vec<bool>, fold_coordinate: [usize; 2], dim: &[usize; 2]) -> (Vec<bool>, [usize; 2]) {

  let [dimx, dimy] = *dim;

  let newdimx = if fold_coordinate[0] == 0 { dimx } else { fold_coordinate[0] };
  let newdimy = if fold_coordinate[1] == 0 { dimy } else { fold_coordinate[1] };
  let mut sheet_after_fold : Vec<bool> = vec![false; newdimx * newdimy];

  for y in 0..dimy {
    for x in 0..dimx {
      let newx : usize = {
        if fold_coordinate[0] == 0 || x < fold_coordinate[0] {
          x
        } else if x == fold_coordinate[0] {
          continue;
        } else {
          dimx - 1 - x
        }
      };
      let newy : usize = {
        if fold_coordinate[1] == 0 || y < fold_coordinate[1] {
          y
        } else if y == fold_coordinate[1] {
          continue;
        } else {
          dimy - 1 - y
        }
      };
      sheet_after_fold[newx + newdimx * newy] = sheet[x + dimx * y] || sheet[newx + dimx * newy]
    }
  }

  (sheet_after_fold, [newdimx, newdimy])
}


pub fn day13(filename: &CxxString) -> [u64; 2] {

  let input_puzzle = fs::read_to_string(filename.to_str().unwrap()).expect("File not found!");
  let lines: Vec<&str> = input_puzzle.lines().collect();

  let mut dots_coordinate: Vec<[usize; 2]> = Vec::new();
  let mut fold_coordinates: Vec<[usize; 2]> = Vec::new();

  let mut have_finish_dots_parsing : bool = false;

  for line in &lines {
    if line.trim().len() == 0 {
      have_finish_dots_parsing = true;
      continue;
    }

    if !have_finish_dots_parsing {
      dots_coordinate.push(line.split(',')
                               .map(|value| value.parse().expect("Fail to parse dot coordinate"))
                               .collect::<Vec<_>>()
                               .try_into().expect("Don't have proper (x,y) coordinate"));
    } else {
      let fold_line : Vec<&str> = line.split('=').collect();
      let fold_direction = match fold_line[0].split(' ').last().unwrap() {
        "x" => 0,
        "y" => 1,
          _ => panic!("Unkown coordinate")
      };
      let value : usize = fold_line[fold_line.len()-1].parse().expect("Failed to parse fold");
      fold_coordinates.push([if fold_direction == 0 { value } else { 0 }, if fold_direction == 1 { value } else { 0 }]);
    }
  }

  let dimx : usize = dots_coordinate.iter().map(|[x,_]| x).max().unwrap() + 1;
  let dimy : usize = dots_coordinate.iter().map(|[_,y]| y).max().unwrap() + 1;

  let mut sheet : Vec<bool> = vec![false; dimx * dimy];
  for [x, y] in dots_coordinate.iter() {
    sheet[x + dimx * y] = true;
  }

  let (mut sheet_after_fold, [mut dimx, mut dimy]) = fold(sheet, fold_coordinates[0], &[dimx, dimy]);
  let part1 : u64 = sheet_after_fold.iter().filter(|value| **value).count() as u64;

  for fold_coordinate in fold_coordinates[1..].iter() {
    (sheet_after_fold, [dimx, dimy]) = fold(sheet_after_fold, *fold_coordinate, &[dimx, dimy]);
  }
  let part2 : u64 = sheet_after_fold.iter().filter(|value| **value).count() as u64;

  // for y in 0..dimy {
  //   for x in 0..dimx {
  //     print!("{}", if sheet_after_fold[(x + dimx * y) as usize] { "#" } else { "." });
  //   }
  //   println!("");
  // }
  // println!("");

  // The answer is visual
  //    ###..###...##..#..#.####.###..#....###..
  //    #..#.#..#.#..#.#.#..#....#..#.#....#..#.
  //    #..#.#..#.#....##...###..###..#....#..#.
  //    ###..###..#....#.#..#....#..#.#....###..
  //    #.#..#....#..#.#.#..#....#..#.#....#.#..
  //    #..#.#.....##..#..#.#....###..####.#..#.

  [part1, part2]
}