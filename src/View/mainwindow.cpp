#include "mainwindow.h"

#include <QGraphicsRectItem>
#include <QGraphicsScene>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent, s21::Maze *maze)
    : QMainWindow(parent), ui(new Ui::MainWindow), controler(maze) {
  ui->setupUi(this);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_pushButton_clicked() {
  QGraphicsScene* scene = new QGraphicsScene(ui->graphicsView);
  ui->graphicsView->setScene(scene);

  try {
    controler.PerfectMazeGen(ui->textEdit_rows->toPlainText().toInt(),ui->textEdit_cols->toPlainText().toInt());
  } catch(...) {
    return;
  }

  std::vector<std::vector<int>*> right = controler.getRight();

  std::vector<std::vector<int>*> down = controler.getDown();

  int rows = right.size();
  int cols = right[0]->size();

  // Отступы и размеры ячеек
  int xOffset = 30;
  int yOffset = 30;
  int cellSize = ui->graphicsView->size().height() / rows;

  // Отрисовка ячеек лабиринта
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (j == 0) {
        QGraphicsRectItem* rightWall =
            new QGraphicsRectItem(xOffset, i * cellSize + yOffset, 1, cellSize);
        QPen pen;
        pen.setWidth(2);
        rightWall->setPen(pen);
        scene->addItem(rightWall);
      }

      if (i == 0) {
        QGraphicsRectItem* downWall =
            new QGraphicsRectItem(j * cellSize + xOffset, yOffset, cellSize, 1);
        QPen pen;
        pen.setWidth(2);
        downWall->setPen(pen);
        scene->addItem(downWall);
      }

      if (right[i]->operator[](j) == 1) {
        QGraphicsRectItem* rightWall =
            new QGraphicsRectItem(j * cellSize + xOffset + cellSize,
                                  i * cellSize + yOffset, 1, cellSize);
        QPen pen;
        pen.setWidth(2);
        rightWall->setPen(pen);
        scene->addItem(rightWall);
      }

      if (down[i]->operator[](j) == 1) {
        QGraphicsRectItem* downWall = new QGraphicsRectItem(
            j * cellSize + xOffset, i * cellSize + yOffset + cellSize, cellSize,
            1);
        QPen pen;
        pen.setWidth(2);
        downWall->setPen(pen);
        scene->addItem(downWall);
      }
    }
  }
}
