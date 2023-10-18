#include <memory>

#include "../../backend/includes/Maze.h"

namespace s21 {
class Controler {
 public:
  Controler(s21::Maze *new_maze) : maze_{new_maze} {};
  void PerfectMazeGen(int rows, int cols);
  void ReadMaze(std::string fpath);
  std::pair<std::vector<std::vector<int>>, bool> MazeSolve(
      std::vector<std::pair<int, int>> v);
  std::vector<std::vector<int> *> getRight();
  std::vector<std::vector<int> *> getDown();

 private:
  std::unique_ptr<s21::Maze> maze_;
};
}  // namespace s21
