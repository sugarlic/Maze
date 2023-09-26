#include <iostream>

#include "Maze/Maze.h"

int main() {
  int rows{6};
  int cols{6};
  s21::Maze maze;
  maze.PerfectMazeGen(rows, cols);
  std::vector<std::vector<int>*> right = maze.getRight();
  std::vector<std::vector<int>*> down = maze.getDown();

  for (size_t i = 0; i < right.size(); i++) {
    for (size_t j = 0; j < right[0]->size(); j++) {
      std::cout << right[i]->operator[](j) << ' ';
    }
    std::cout << '\n';
  }
  std::cout << '\n' << '\n';
  for (size_t i = 0; i < down.size(); i++) {
    for (size_t j = 0; j < down[0]->size(); j++) {
      std::cout << down[i]->operator[](j) << ' ';
    }
    std::cout << '\n';
  }
  return 0;
}