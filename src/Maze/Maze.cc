#include "Maze.h"

#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
// ИЗМЕНИТЬ СОХРАНЕНИЕ ФАЙЛА

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
        right_[i]->operator[](j) = 1;
      } else {
        if (matrix[i][j] == matrix[i][j + 1]) {
          right_[i]->operator[](j) = 1;
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
      if (i == rows - 1) down_[i]->operator[](j) = 1;
      int r_num{std::rand() % 2};
      if (r_num) {
        int num = matrix[i][j];
        int count{};
        for (int z = 0; z < cols; z++) {
          if (matrix[i][z] == num && !down_[i]->operator[](z)) count++;
        }
        if (count > 1) down_[i]->operator[](j) = 1;
      }
    }
    //копирование строки
    if (i != rows - 1)
      for (int j = 0; j < cols; j++) {
        if (!down_[i]->operator[](j)) matrix[i + 1][j] = matrix[i][j];
      }
    else {
      for (int j = 0; j < cols - 1; j++) {
        if (matrix[i][j] != matrix[i][j + 1]) right_[i]->operator[](j) = 0;
        int num = matrix[i][j + 1];
        for (int z = 0; z < cols; z++) {
          if (matrix[i][z] == num) matrix[i][z] = matrix[i][j];
        }
      }
    }
  }

  SaveMaze();
}

void s21::Maze::ReadMaze(std::string fpath) {
  std::ifstream fin(fpath);
  if (!fin.is_open()) throw std::invalid_argument("INCORRECT FILENAME");
  int rows, cols;
  fin >> rows >> cols;
  if (rows < 1 || rows > 50 || cols < 1 || cols > 50)
    throw std::invalid_argument("INCORRECT FILE");

  MatrixInitialization(rows, cols);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      fin >> right_[i]->operator[](j);
    }
  }

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      fin >> down_[i]->operator[](j);
    }
  }

  fin.close();
}

void s21::Maze::SaveMaze() {
  if (right_.empty()) return;
  auto now = std::chrono::system_clock::now();
  std::time_t time = std::chrono::system_clock::to_time_t(now);
  std::stringstream stream;
  stream << std::put_time(std::localtime(&time), "%H:%M:%S");
  std::string date_string = stream.str();
  std::string fname = "/Users/sullustd/Maze/src/Examples/" + date_string +
                      ".txt";  // путь к файлу (изменить!)

  std::ofstream file(fname);
  file << right_.size() << ' ' << right_[0]->size() << '\n';
  for (size_t i = 0; i < right_.size(); i++) {
    for (size_t j = 0; j < right_[0]->size(); j++) {
      file << right_[i]->operator[](j) << ' ';
    }
    file << '\n';
  }
  file << '\n';
  for (size_t i = 0; i < down_.size(); i++) {
    for (size_t j = 0; j < down_[0]->size(); j++) {
      file << down_[i]->operator[](j) << ' ';
    }
    file << '\n';
  }
  file.close();
}

void s21::Maze::MatrixInitialization(int rows, int cols) {
  right_.resize(rows);
  down_.resize(rows);
  for (int i = 0; i < rows; i++) {
    right_[i] = new std::vector<int>(cols);
    down_[i] = new std::vector<int>(cols);
  }
}
