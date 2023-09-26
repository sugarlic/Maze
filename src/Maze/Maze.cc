#include "Maze.h"

#include <ctime>
#include <iostream>

void s21::Maze::PerfectMazeGen(int rows, int cols) {
  if ((rows <= 0 || cols <= 0) || rows > 50 || cols > 50)
    throw std::invalid_argument("ERROR");
  std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols));
  MatrixInitialization(rows, cols);
  std::srand(std::time(0));

  int k{1};
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (matrix[i][j] == 0) {
        matrix[i][j] = k;
        k++;
      }
    }
    // правые стенки
    for (int j = 0; j < cols; j++) {
      int r_num{std::rand() % 2};
      if (r_num || j == cols - 1) {
        right[i]->operator[](j) = 1;
      } else {
        if (matrix[i][j] == matrix[i][j + 1]) {
          right[i]->operator[](j) = 1;
        } else {
          int num = matrix[i][j + 1];
          for (int z = 0; z < cols; z++) {
            if (matrix[i][z] == num) matrix[i][z] = matrix[i][j];
          }
        }
      }
    }
    //левые стенки
    for (int j = 0; j < cols; j++) {
      if (i == rows - 1) down[i]->operator[](j) = 1;
      int r_num{std::rand() % 2};
      if (r_num) {
        int num = matrix[i][j];
        int count{};
        for (int z = 0; z < cols; z++) {
          if (matrix[i][z] == num && !down[i]->operator[](z)) count++;
        }
        if (count > 1) down[i]->operator[](j) = 1;
      }
    }
    //копирование строки
    if (i != rows - 1)
      for (int j = 0; j < cols; j++) {
        if (!down[i]->operator[](j)) matrix[i + 1][j] = matrix[i][j];
      }
    else {
      for (int j = 0; j < cols - 1; j++) {
        if (matrix[i][j] != matrix[i][j + 1]) right[i]->operator[](j) = 0;
        int num = matrix[i][j + 1];
        for (int z = 0; z < cols; z++) {
          if (matrix[i][z] == num) matrix[i][z] = matrix[i][j];
        }
      }
    }
  }
}

void s21::Maze::MatrixInitialization(int rows, int cols) {
  right.resize(rows);
  down.resize(rows);
  for (int i = 0; i < rows; i++) {
    right[i] = new std::vector<int>(cols);
    down[i] = new std::vector<int>(cols);
  }
}
