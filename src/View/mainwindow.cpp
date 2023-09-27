#include "mainwindow.h"

#include "ui_mainwindow.h"
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QMouseEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent, s21::Maze* maze)
    : QMainWindow(parent), ui(new Ui::MainWindow), controler_(maze) {
  ui->setupUi(this);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_pushButton_clicked() {
  QGraphicsScene* scene = new QGraphicsScene(ui->graphicsView);
  ui->graphicsView->setScene(scene);

  try {
    controler_.PerfectMazeGen(ui->textEdit_rows->toPlainText().toInt(),
                             ui->textEdit_cols->toPlainText().toInt());
  } catch (...) {
    return; // исправить!!!
  }

  right_ = controler_.getRight();
  down_ = controler_.getDown();
  MazeDraw(scene);
}

void MainWindow::on_ReadMaze_clicked()
{
    point_count_ = 0;
    QFileDialog fileDialog;
    QString filename;
    fileDialog.setWindowTitle("Выберите директорию");
    fileDialog.setFileMode(QFileDialog::ExistingFile);

    fileDialog.setDirectory("../../../../");

    if (fileDialog.exec()) {
      filename = fileDialog.selectedFiles().at(0);
    }
    QGraphicsScene* scene = new QGraphicsScene(ui->graphicsView);
    ui->graphicsView->setScene(scene);

    try {
        controler_.ReadMaze(filename.toStdString());
    } catch(...) {
        return; // исправить!!!
    }

    right_ = controler_.getRight();
    down_ = controler_.getDown();
    MazeDraw(scene);
}

void MainWindow::MazeDraw(QGraphicsScene *scene){
    point_count_ = 0;
    int rows = right_.size();
    int cols = right_[0]->size();

    int xOffset = 0;
    int yOffset = 0;
    int cellSize = ui->graphicsView->size().height() / rows -
                   (ui->graphicsView->size().height() / rows) / 10;
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::gray);
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        if (j == 0) {
          QGraphicsRectItem* rightWall =
              new QGraphicsRectItem(xOffset, i * cellSize + yOffset, 1, cellSize);
          rightWall->setPen(pen);
          scene->addItem(rightWall);
        }

        if (i == 0) {
          QGraphicsRectItem* downWall =
              new QGraphicsRectItem(j * cellSize + xOffset, yOffset, cellSize, 1);
          downWall->setPen(pen);
          scene->addItem(downWall);
        }

        if (right_[i]->operator[](j) == 1) {
          QGraphicsRectItem* rightWall =
              new QGraphicsRectItem(j * cellSize + xOffset + cellSize,
                                    i * cellSize + yOffset, 1, cellSize);
          rightWall->setPen(pen);
          scene->addItem(rightWall);
        }

        if (down_[i]->operator[](j) == 1) {
          QGraphicsRectItem* downWall = new QGraphicsRectItem(
              j * cellSize + xOffset, i * cellSize + yOffset + cellSize, cellSize,
              1);
          downWall->setPen(pen);
          scene->addItem(downWall);
        }
      }
    }
}

void MainWindow::mousePressEvent(QMouseEvent* event){
    if (event->button() != Qt::LeftButton) return;
    QPointF pos = event->pos();


    if (pos.x()  > 10 && pos.x() < 500 && pos.y() > 10 && pos.y() < 500) {
        if (point_count_ > 1) {
            begin_end_.erase(begin_end_.begin());
            ui->graphicsView->scene()->removeItem(*(++ui->graphicsView->scene()->items().begin()));
        }

        int cellSize = ui->graphicsView->size().height() / right_.size() -
                       (ui->graphicsView->size().height() / right_.size()) / 10;
        int x_pos = ui->graphicsView->scene()->width() - 490 + pos.x();
        int y_pos = ui->graphicsView->scene()->height() - 490 + pos.y();
        x_pos = (x_pos - x_pos % cellSize) + cellSize/2;
        y_pos = (y_pos - y_pos % cellSize) + cellSize/2;
        QGraphicsRectItem *point = new QGraphicsRectItem(x_pos, y_pos,2,2);
        QPen point_pen;
        point_pen.setWidth(2);
        if (point_count_ % 2)
            point_pen.setColor(Qt::red);
        else
            point_pen.setColor(Qt::green);
        point->setPen(point_pen);
        ui->graphicsView->scene()->addItem(point);

        point_count_++;
        begin_end_.push_back(std::make_pair(x_pos,y_pos));
    }
}
