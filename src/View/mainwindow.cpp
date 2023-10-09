#include "mainwindow.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QMouseEvent>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent, s21::Maze* maze)
    : QMainWindow(parent), ui(new Ui::MainWindow), controler_(maze) {
  ui->setupUi(this);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_pushButton_clicked() {
  try {
    controler_.PerfectMazeGen(ui->textEdit_rows->toPlainText().toInt(),
                              ui->textEdit_cols->toPlainText().toInt());
  } catch (...) {
    return;  // исправить!!!
  }

  ClearSolution();
  ClearPoints();
  QGraphicsScene* scene = new QGraphicsScene(ui->graphicsView);
  ui->graphicsView->setScene(scene);

  right_ = controler_.getRight();
  down_ = controler_.getDown();
  MazeDraw(scene);
}

void MainWindow::on_ReadMaze_clicked() {
  QFileDialog fileDialog;
  QString filename;
  fileDialog.setWindowTitle("Выберите директорию");
  fileDialog.setFileMode(QFileDialog::ExistingFile);

  fileDialog.setDirectory("../../../../");

  if (fileDialog.exec()) {
    filename = fileDialog.selectedFiles().at(0);
  }

  try {
    controler_.ReadMaze(filename.toStdString());
  } catch (...) {
    return;  // исправить!!!
  }

  ClearSolution();
  ClearPoints();
  QGraphicsScene* scene = new QGraphicsScene(ui->graphicsView);
  ui->graphicsView->setScene(scene);

  right_ = controler_.getRight();
  down_ = controler_.getDown();
  MazeDraw(scene);
}

void MainWindow::MazeDraw(QGraphicsScene* scene) {
  int rows = right_.size();
  int cols = right_[0]->size();

  int xOffset = 0;
  int yOffset = 0;
  int cell_widht = ui->graphicsView->size().width() / cols -
                 (ui->graphicsView->size().width() / cols) / 10;
  int cell_height = ui->graphicsView->size().height() / rows -
                 (ui->graphicsView->size().height() / rows) / 10;
  QPen pen;
  pen.setWidth(2);
  pen.setColor(Qt::gray);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (j == 0) {
        QGraphicsRectItem* rightWall =
            new QGraphicsRectItem(xOffset, i * cell_height + yOffset, 1, cell_height);
        rightWall->setPen(pen);
        scene->addItem(rightWall);
      }

      if (i == 0) {
        QGraphicsRectItem* downWall =
            new QGraphicsRectItem(j * cell_widht + xOffset, yOffset, cell_widht, 1);
        downWall->setPen(pen);
        scene->addItem(downWall);
      }

      if (right_[i]->operator[](j) == 1) {
        QGraphicsRectItem* rightWall =
            new QGraphicsRectItem(j * cell_widht + xOffset + cell_widht,
                                  i * cell_height + yOffset, 1, cell_height);
        rightWall->setPen(pen);
        scene->addItem(rightWall);
      }

      if (down_[i]->operator[](j) == 1) {
        QGraphicsRectItem* downWall =
                new QGraphicsRectItem(j * cell_widht + xOffset, i * cell_height + yOffset + cell_height,
                                      cell_widht, 1);
        downWall->setPen(pen);
        scene->addItem(downWall);
      }
    }
  }
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
  if (event->button() != Qt::LeftButton || !ui->graphicsView->scene()
          || ui->graphicsView->size().width() <= 0 || ui->graphicsView->size().height() <= 0) return;
  QPoint pos = event->pos();
  qDebug() << right_.size()<<right_[0]->size();
  int cell_widht = ui->graphicsView->size().width() / right_[0]->size() -
                 (ui->graphicsView->size().width() / right_[0]->size()) / 10;
  int cell_height = ui->graphicsView->size().height() / right_.size() -
                 (ui->graphicsView->size().height() / right_.size()) / 10;

  if (pos.x() > cell_widht/2 &&
          pos.x() < ui->graphicsView->size().width() - cell_widht/2 &&
          pos.y() > cell_height/2 &&
          pos.y() < ui->graphicsView->size().height() - cell_height/2) {

    if (point_items_.size() > 1) {
      begin_end_.erase(begin_end_.begin());
      ui->graphicsView->scene()->removeItem(point_items_.front());
      point_items_.pop();

      while(solution_items_.size() > 0) {
          ui->graphicsView->scene()->removeItem(solution_items_.top());
          delete solution_items_.top();
          solution_items_.pop();
      }
    }

    int x_pos = ui->graphicsView->scene()->width() - 500 + pos.x();
    int y_pos = ui->graphicsView->scene()->height() - 500 + pos.y();
    x_pos = (x_pos - x_pos % cell_widht) + cell_widht / 2;
    y_pos = (y_pos - y_pos % cell_height) + cell_height / 2;

    QGraphicsRectItem* point = new QGraphicsRectItem(x_pos, y_pos, 2, 2);
    QPen point_pen;
    point_pen.setWidth(2);
    if (color_ % 2)
      point_pen.setColor(Qt::red);
    else
      point_pen.setColor(Qt::green);
    point->setPen(point_pen);
    ui->graphicsView->scene()->addItem(point);

    point_items_.push(point);
    ++color_;
    begin_end_.push_back(std::make_pair(y_pos/cell_height, x_pos/cell_widht));
  }
}

void MainWindow::DrawMazeSolution(std::vector<std::vector<int>> v) {
    while(solution_items_.size() > 0) {
        ui->graphicsView->scene()->removeItem(solution_items_.top());
        delete solution_items_.top();
        solution_items_.pop();
    }
    int cell_widht = ui->graphicsView->size().width() / right_[0]->size() -
                   (ui->graphicsView->size().width() / right_[0]->size()) / 10;
    int cell_height = ui->graphicsView->size().height() / right_.size() -
                   (ui->graphicsView->size().height() / right_.size()) / 10;
    int i = begin_end_.back().first;
    int j = begin_end_.back().second;
    QPen pen;
    pen.setWidth(1);
    pen.setColor(Qt::red);
    while(v[i][j] != 0) {
        if (i > 0 && down_[i - 1]->operator[](j) != 1 && v[i - 1][j] == v[i][j] - 1) {
            QGraphicsRectItem* right_wall =
                new QGraphicsRectItem(j * cell_widht + cell_widht/2,
                                      i * cell_height - cell_height/2, 1, cell_height);
            right_wall->setPen(pen);
            ui->graphicsView->scene()->addItem(right_wall);
            solution_items_.push(right_wall);
            i--;
        }
        else if (i < right_.size() - 1 && down_[i]->operator[](j) != 1 && v[i + 1][j] == v[i][j] - 1) {
            QGraphicsRectItem* right_wall =
                new QGraphicsRectItem(j * cell_widht + cell_widht/2,
                                      i * cell_height + cell_height/2, 1, cell_height);
            right_wall->setPen(pen);
            ui->graphicsView->scene()->addItem(right_wall);
            solution_items_.push(right_wall);
            i++;
        }
        else if (j > 0 && right_[i]->operator[](j - 1) != 1 && v[i][j - 1] == v[i][j] - 1) {
            QGraphicsRectItem* down_wall =
                new QGraphicsRectItem(j * cell_widht - cell_widht/2,
                                      i * cell_height + cell_height/2, cell_widht, 1);
            down_wall->setPen(pen);
            ui->graphicsView->scene()->addItem(down_wall);
            solution_items_.push(down_wall);
            j--;
        }
        else if (j < right_[0]->size() - 1 && right_[i]->operator[](j) != 1 && v[i][j + 1] == v[i][j] - 1) {
            QGraphicsRectItem* down_wall =
                new QGraphicsRectItem(j * cell_widht + cell_widht/2,
                                      i * cell_height + cell_height/2, cell_widht, 1);
            down_wall->setPen(pen);
            ui->graphicsView->scene()->addItem(down_wall);
            solution_items_.push(down_wall);
            j++;
        }
    }

}

void MainWindow::on_pushButton_Maze_Solve_clicked()
{
    if (begin_end_.size() != 2) return;
    if (IsCorrectPoints()) {
        std::vector<std::vector<int>>res = controler_.MazeSolve(begin_end_);
        DrawMazeSolution(res);
    }
}

bool MainWindow::IsCorrectPoints() {
    for (int i = 0; i < 2; i++) {
        if (begin_end_[i].first < 0 || begin_end_[i].first >= right_.size()
                || begin_end_[i].second < 0 || begin_end_[i].second >= right_[0]->size()) {
            return false;
        }
    }
    return true;
}

void MainWindow::ClearSolution() {
    while(!solution_items_.empty()) {
        delete solution_items_.top();
        solution_items_.pop();
    }
}

void MainWindow::ClearPoints() {
    begin_end_.clear();
    while(!point_items_.empty()) {
        point_items_.pop();
    }
}
