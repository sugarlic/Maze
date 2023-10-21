#include <gtest/gtest.h>

#include <vector>

#include "../../backend/includes/Maze.h"

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

TEST(Maze_test, test_wrong_solve5) {
  s21::Maze maze;
  std::vector<std::pair<int, int>> v = {{1, 1}, {2, 2}};
  EXPECT_ANY_THROW({ maze.MazeSolve(v); });
}

TEST(Maze_test, test_wrong_solve6) {
  s21::Maze maze;
  maze.PerfectMazeGen(6, 6);
  std::vector<std::pair<int, int>> v = {{1, 1}, {2, 2}};
  EXPECT_NO_THROW({ maze.MazeSolve(v); });
}

TEST(Maze_test, test_copy) {
  s21::Maze maze1;
  maze1.PerfectMazeGen(6, 7);
  s21::Maze maze2{maze1};

  std::vector<std::vector<int> *> right1{maze1.getRight()};
  std::vector<std::vector<int> *> down1{maze1.getDown()};
  std::vector<std::vector<int> *> right2{maze2.getRight()};
  std::vector<std::vector<int> *> down2{maze2.getDown()};
  EXPECT_EQ(right1.size(), right2.size());
  EXPECT_EQ(right1[0]->size(), right2[0]->size());
  EXPECT_EQ(down1.size(), down2.size());
  EXPECT_EQ(down1[0]->size(), down2[0]->size());
  for (size_t i = 0; i < right1.size(); i++) {
    for (size_t j = 0; j < right1[0]->size(); j++) {
      EXPECT_EQ(right1[i]->operator[](j), right2[i]->operator[](j));
    }
  }
  for (size_t i = 0; i < down1.size(); i++) {
    for (size_t j = 0; j < down1[0]->size(); j++) {
      EXPECT_EQ(down1[i]->operator[](j), down2[i]->operator[](j));
    }
  }
}

TEST(Maze_test, test_move) {
  s21::Maze maze1;
  maze1.PerfectMazeGen(6, 7);
  s21::Maze maze2;
  maze2 = std::move(maze1);

  std::vector<std::vector<int> *> right1{maze1.getRight()};
  std::vector<std::vector<int> *> down1{maze1.getDown()};

  for (size_t i = 0; i < right1.size(); i++) {
    for (size_t j = 0; j < right1[0]->size(); j++) {
      EXPECT_EQ(right1[i]->operator[](j), 0);
    }
  }
  for (size_t i = 0; i < down1.size(); i++) {
    for (size_t j = 0; j < down1[0]->size(); j++) {
      EXPECT_EQ(down1[i]->operator[](j), 0);
    }
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
