#include <vector>

namespace s21 {
class Maze {
 public:
  void PerfectMazeGen(int rows, int cols);
  std::vector<std::vector<int>*> getRight() { return right; };
  std::vector<std::vector<int>*> getDown() { return down; };

 private:
  std::vector<std::vector<int>*> right;
  std::vector<std::vector<int>*> down;
  void MatrixInitialization(int rows, int cols);
};
}  // namespace s21