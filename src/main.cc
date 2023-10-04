#include <iostream>

#include "Maze/Maze.h"

int main() {
  s21::Maze maze;
  maze.ReadMaze("/Users/sullustd/Maze/src/Examples/1.txt");
  std::vector<std::pair<int, int>> v{{0, 0}, {5, 5}};
  std::vector<std::vector<int>> res = maze.MazeSolve(v);

  for (size_t i = 0; i < res.size(); i++) {
    for (size_t j = 0; j < res[0].size(); j++) {
      std::cout << res[i][j] << ' ';
    }
    std::cout << '\n';
  }
  return 0;
}