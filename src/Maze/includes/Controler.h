#include "../Maze/Maze.h"

namespace s21 {
class Controler {
 public:
  Controler(s21::Maze *new_maze) : maze_{new_maze} {};
  void PerfectMazeGen(int rows, int cols);
  void ReadMaze(std::string fpath);
  std::vector<std::vector<int>> MazeSolve(std::vector<std::pair<int, int>> v);
  std::vector<std::vector<int> *> getRight();
  std::vector<std::vector<int> *> getDown();

 private:
  s21::Maze *maze_;
};
}  // namespace s21