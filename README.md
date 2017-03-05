# Sliding Puzzle
A sliding puzzle game and solver using ncurses to display the grid. It will solve for 3x3 puzzles instantly, and will solve for most 4x4 puzzles in a moderate amount of time.

# Algorithm
The solution uses [Iterative Deepening A* (IDA*)](https://en.wikipedia.org/wiki/Iterative_deepening_A*) with the following heuristics:
- [Hamming distance](https://en.wikipedia.org/wiki/Hamming_distance)
- [Manhattan distance](https://en.wikipedia.org/wiki/Taxicab_geometry)
- [Linear conflict](https://heuristicswiki.wikispaces.com/Linear+Conflict)

# Dependencies
- [ncurses 6.0](https://www.gnu.org/software/ncurses/)

# License
This project is released under MIT License. Please review the [License file](LICENSE) for more details.
