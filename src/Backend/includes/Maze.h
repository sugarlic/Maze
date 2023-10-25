#include <memory>
#include <string>
#include <vector>

namespace s21 {
class Maze {
 public:
  Maze() = default;
  Maze(const Maze &other);
  Maze(Maze &&other);
  void PerfectMazeGen(int rows, int cols);
  std::vector<std::vector<int> *> getRight() { return right_; };
  std::vector<std::vector<int> *> getDown() { return down_; };
  void ReadMaze(std::string fpath);
  std::pair<std::vector<std::vector<int>>, bool> MazeSolve(
      std::vector<std::pair<int, int>> v);
  Maze operator=(const Maze &other);
  Maze operator=(Maze &&other);
  ~Maze();

 private:
  std::vector<std::vector<int> *> right_;
  std::vector<std::vector<int> *> down_;
  void MatrixInitialization(int rows, int cols);
  void SaveMaze();
  void StepWave(std::vector<std::vector<int>> &path, size_t i, size_t j,
                int step);
  void ClearMaze();
};
}  // namespace s21