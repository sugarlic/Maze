#include <gtest/gtest.h>

#include "../Backend/includes/Maze.h"

TEST(Maze_test, test_wrong_size1) {
  s21::Maze maze;
  EXPECT_ANY_THROW({ maze.PerfectMazeGen(0, 0); });
}

TEST(Maze_test, test_wrong_size2) {
  s21::Maze maze;
  EXPECT_ANY_THROW({ maze.PerfectMazeGen(-6, 2); });
}

TEST(Maze_test, test_wrong_size3) {
  s21::Maze maze;
  EXPECT_ANY_THROW({ maze.PerfectMazeGen(6, -6); });
}

TEST(Maze_test, test_wrong_size4) {
  s21::Maze maze;
  EXPECT_ANY_THROW({ maze.PerfectMazeGen(-6, -6); });
}

TEST(Maze_test, test_wrong_solve) {
  s21::Maze maze;
  std::vector<std::pair<int, int>> v = {{1, 1}, {2, 2}};
  EXPECT_ANY_THROW({ maze.MazeSolve(v); });
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
