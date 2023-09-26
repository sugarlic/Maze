#include "../Maze/Maze.h"

namespace s21 {
class Controler {
 public:
  Controler(s21::Maze *new_maze) : maze{new_maze} {};
  void PerfectMazeGen(int rows, int cols);
  std::vector<std::vector<int> *> getRight();
  std::vector<std::vector<int> *> getDown();

 private:
  s21::Maze *maze;
};
}  // namespace s21
