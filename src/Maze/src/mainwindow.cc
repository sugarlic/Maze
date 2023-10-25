#include "mainwindow.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QMouseEvent>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent, s21::Maze* maze)
    : QMainWindow(parent), ui(new Ui::MainWindow), controler_(maze) {
  ui->setupUi(this);
  validator_ = new QIntValidator(1, 50, this);
  ui->textEdit_cols->setValidator(validator_);
  ui->textEdit_rows->setValidator(validator_);
}

MainWindow::~MainWindow() {
  delete ui;
  delete validator_;
}

void MainWindow::on_pushButton_clicked() {
  try {
    controler_.PerfectMazeGen(ui->textEdit_rows->text().toInt(),
                              ui->textEdit_cols->text().toInt());
  } catch (...) {
    QMessageBox msg;
    msg.setText(
        "Maze doesn't generated. Maybe you have not need free memory or your "
        "maze more than 50x50. Please, "
        "try again.");
    msg.exec();
    return;
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
  QFileDialog file_dialog;
  QString filename;
  file_dialog.setWindowTitle("Выберите директорию");
  file_dialog.setFileMode(QFileDialog::ExistingFile);

  if (file_dialog.exec()) filename = file_dialog.selectedFiles().at(0);

  try {
    controler_.ReadMaze(filename.toStdString());
  } catch (std::invalid_argument& exception) {
    QMessageBox msg;
    msg.setText("Something error. Error type is " + QString(exception.what()));
    msg.exec();
    return;
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
  QSize widget_size = ui->graphicsView->size();
  int rows = right_.size();
  int cols = right_[0]->size();

  int x_offset{};
  int y_offset{};
  int cell_widht =
      widget_size.width() / cols - (widget_size.width() / cols) / 10;
  int cell_height =
      widget_size.height() / rows - (widget_size.height() / rows) / 10;
  QPen pen;
  pen.setWidth(2);
  pen.setColor(Qt::gray);
  QGraphicsRectItem* wall{};
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (j == 0) {
        wall = new QGraphicsRectItem(x_offset, i * cell_height + y_offset, 1,
                                     cell_height);
        DrawSolutionLines(*wall, pen, *scene, false);
      }

      if (i == 0) {
        wall = new QGraphicsRectItem(j * cell_widht + x_offset, y_offset,
                                     cell_widht, 1);
        DrawSolutionLines(*wall, pen, *scene, false);
      }

      if (right_[i]->operator[](j)) {
        wall =
            new QGraphicsRectItem(j * cell_widht + x_offset + cell_widht,
                                  i * cell_height + y_offset, 1, cell_height);
        DrawSolutionLines(*wall, pen, *scene, false);
      }

      if (down_[i]->operator[](j)) {
        wall = new QGraphicsRectItem(j * cell_widht + x_offset,
                                     i * cell_height + y_offset + cell_height,
                                     cell_widht, 1);
        DrawSolutionLines(*wall, pen, *scene, false);
      }
    }
  }
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
  QSize widget_size = ui->graphicsView->size();
  QGraphicsScene* scene = ui->graphicsView->scene();

  if (event->button() != Qt::LeftButton) return;
  if (!scene) return;
  if (widget_size.width() <= 0 || widget_size.height() <= 0) return;

  QPoint pos = event->pos();
  int cell_widht = widget_size.width() / right_[0]->size() -
                   (widget_size.width() / right_[0]->size()) / 10;
  int cell_height = widget_size.height() / right_.size() -
                    (widget_size.height() / right_.size()) / 10;

  if (pos.x() > cell_widht / 2 &&
      pos.x() < widget_size.width() - cell_widht / 2 &&
      pos.y() > cell_height / 2 &&
      pos.y() < widget_size.height() - cell_height / 2) {
    if (point_items_.size() > 1) {
      begin_end_.erase(begin_end_.begin());
      scene->removeItem(point_items_.front());
      delete point_items_.front();
      point_items_.pop();

      ClearSolution();
    }

    // понятия не имею откуда взялись эти сдвиги, хакер чзх
    constexpr int width_offset{590};
    constexpr int height_offset{510};

    int x_pos = scene->width() - width_offset + pos.x();
    int y_pos = scene->height() - height_offset + pos.y();
    x_pos = (x_pos - x_pos % cell_widht) + cell_widht / 2;
    y_pos = (y_pos - y_pos % cell_height) + cell_height / 2;

    if (x_pos < 0 || y_pos < 0) return;
    if (x_pos >= scene->width() || y_pos >= scene->width()) return;

    QGraphicsRectItem* point = new QGraphicsRectItem(x_pos, y_pos, 2, 2);
    QPen point_pen;
    point_pen.setWidth(2);
    QColor current_color = color_ % 2 ? Qt::red : Qt::green;
    point_pen.setColor(current_color);
    point->setPen(point_pen);
    scene->addItem(point);

    point_items_.push(point);
    ++color_;
    begin_end_.emplace_back(
        std::make_pair(y_pos / cell_height, x_pos / cell_widht));
  }
}

void MainWindow::DrawMazeSolution(std::vector<std::vector<int>> v) {
  QSize widget_size = ui->graphicsView->size();
  QGraphicsScene* scene = ui->graphicsView->scene();
  ClearSolution();

  int cell_width = widget_size.width() / right_[0]->size() -
                   (widget_size.width() / right_[0]->size()) / 10;
  int cell_height = widget_size.height() / right_.size() -
                    (widget_size.height() / right_.size()) / 10;
  size_t i = begin_end_.back().first;
  size_t j = begin_end_.back().second;
  QPen pen;
  pen.setWidth(1);
  pen.setColor(Qt::red);
  QGraphicsRectItem* wall{};
  while (v[i][j]) {
    unsigned long rect_x{j * cell_width};
    unsigned long rect_y{i * cell_height};

    if (i > 0 && down_[i - 1]->operator[](j) != 1 &&
        v[i - 1][j] == v[i][j] - 1) {
      wall = new QGraphicsRectItem(rect_x + cell_width / 2,
                                   rect_y - cell_height / 2, 1, cell_height);
      DrawSolutionLines(*wall, pen, *scene, true);
      --i;
    } else if (i < right_.size() - 1 && down_[i]->operator[](j) != 1 &&
               v[i + 1][j] == v[i][j] - 1) {
      wall = new QGraphicsRectItem(rect_x + cell_width / 2,
                                   rect_y + cell_height / 2, 1, cell_height);
      DrawSolutionLines(*wall, pen, *scene, true);
      ++i;
    } else if (j > 0 && right_[i]->operator[](j - 1) != 1 &&
               v[i][j - 1] == v[i][j] - 1) {
      wall = new QGraphicsRectItem(rect_x - cell_width / 2,
                                   rect_y + cell_height / 2, cell_width, 1);
      DrawSolutionLines(*wall, pen, *scene, true);
      --j;
    } else if (j < right_[0]->size() - 1 && right_[i]->operator[](j) != 1 &&
               v[i][j + 1] == v[i][j] - 1) {
      wall = new QGraphicsRectItem(rect_x + cell_width / 2,
                                   rect_y + cell_height / 2, cell_width, 1);
      DrawSolutionLines(*wall, pen, *scene, true);
      ++j;
    }
  }
}

void MainWindow::DrawSolutionLines(QGraphicsRectItem& qgritem, QPen& pen,
                                   QGraphicsScene& scene, bool to_push) {
  qgritem.setPen(pen);
  scene.addItem(&qgritem);
  if (to_push) solution_items_.push(&qgritem);
}

void MainWindow::on_pushButton_Maze_Solve_clicked() {
  if (begin_end_.size() != 2) return;
  if (IsCorrectPoints()) {
    std::pair<std::vector<std::vector<int>>, bool> res =
        controler_.MazeSolve(begin_end_);
    if (res.second) DrawMazeSolution(res.first);
  }
}

bool MainWindow::IsCorrectPoints() {
  for (int i = 0; i < 2; i++) {
    if (begin_end_[i].first < 0 ||
        begin_end_[i].first >= static_cast<int>(right_.size()) ||
        begin_end_[i].second < 0 ||
        begin_end_[i].second >= static_cast<int>(right_[0]->size())) {
      return false;
    }
  }
  return true;
}

void MainWindow::ClearSolution() {
  while (!solution_items_.empty()) {
    if (!solution_items_.empty())
      ui->graphicsView->scene()->removeItem(solution_items_.top());
    delete solution_items_.top();
    solution_items_.pop();
  }
}

void MainWindow::ClearPoints() {
  begin_end_.clear();
  while (!point_items_.empty()) {
    point_items_.pop();
  }
}
