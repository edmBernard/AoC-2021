use cxx::CxxString;

fn get_rule(input: &[u32], index: u32) -> u32 {
  input.iter().filter(|x| (x.clone() & (1 << index)) != 0).count() as u32
}

pub fn day03functional(filename: &CxxString, part1: &mut u64, part2: &mut u64) {

  let mut line_length = 0;

  let mut input_puzzle = std::fs::read_to_string(filename.to_str().unwrap())
    .expect("File not found!")
    .lines()
    .map(|x| {
      // I don't find a proper to do that in "rust"
      if line_length == 0 {
        line_length = x.chars().count()
      };
      x
    })
    .map(|x| u32::from_str_radix(x, 2).unwrap())
    .collect::<Vec<_>>();

  input_puzzle.sort();

  let mut gamma = 0;
  for i in 0..line_length as u32 {
    gamma += if get_rule(&input_puzzle, i) > input_puzzle.len() as u32 / 2 {
      1 << i
    } else {
      0
    }
  }
  let epsilon = (1 << line_length)-1 & !gamma;

  *part1 = gamma * epsilon;
  *part2 = 0;
}


// uint64_t treeDescent(const std::vector<uint16_t> &inputPuzzle, size_t lineLength, bool option) {
//   uint64_t result = 0;
//   auto begin = inputPuzzle.begin();
//   auto end = inputPuzzle.end();
//   // We reduce the search at each iteration by moving begin and end of the search
//   for (size_t i = 0, shift = lineLength -1; i < lineLength; ++i, --shift) {
//     auto pos = std::find_if(begin, end, [&](auto value) { return (value >> shift) & 1; });
//     if (pos != end) {
//       if ((pos - begin <= end - pos) ^ option) {
//         begin = pos;
//       } else {
//         end = pos;
//       }
//       result = *pos;
//     }
//   }
//   return result;
// }
