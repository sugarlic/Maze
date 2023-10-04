#include "Controler.h"

void s21::Controler::PerfectMazeGen(int rows, int cols) {
  maze_->PerfectMazeGen(rows, cols);
}

std::vector<std::vector<int> *> s21::Controler::getRight() {
  return maze_->getRight();
}

std::vector<std::vector<int> *> s21::Controler::getDown() {
  return maze_->getDown();
}

void s21::Controler::ReadMaze(std::string fpath) { maze_->ReadMaze(fpath); }

std::vector<std::vector<int>> s21::Controler::MazeSolve(
    std::vector<std::pair<int, int>> v) {
  return maze_->MazeSolve(v);
}