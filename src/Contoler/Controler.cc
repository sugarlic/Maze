#include "Controler.h"

void s21::Controler::PerfectMazeGen(int rows, int cols) {
  maze->PerfectMazeGen(rows, cols);
}

std::vector<std::vector<int> *> s21::Controler::getRight() {
  return maze->getRight();
}

std::vector<std::vector<int> *> s21::Controler::getDown() {
  return maze->getDown();
}

void s21::Controler::ReadMaze(std::string fpath) { maze->ReadMaze(fpath); }