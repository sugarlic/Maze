#include <string>
#include <vector>

namespace s21 {
class Maze {
 public:
  void PerfectMazeGen(int rows, int cols);
  std::vector<std::vector<int>*> getRight() { return right_; };
  std::vector<std::vector<int>*> getDown() { return down_; };
  void ReadMaze(std::string fpath);

 private:
  std::vector<std::vector<int>*> right_;
  std::vector<std::vector<int>*> down_;
  void MatrixInitialization(int rows, int cols);
  void SaveMaze();
};
}  // namespace s21