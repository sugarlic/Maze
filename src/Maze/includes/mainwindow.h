#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QMainWindow>
#include <QIntValidator>
#include <queue>
#include <stack>

#include "Controler.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr, s21::Maze *maze = nullptr);
  ~MainWindow();

 protected:
  void mousePressEvent(QMouseEvent *event) override;

 private slots:
  void on_pushButton_clicked();

  void on_ReadMaze_clicked();

  void on_pushButton_Maze_Solve_clicked();

 private:
  Ui::MainWindow *ui;
  s21::Controler controler_;
  QIntValidator* validator_{};
  std::vector<std::vector<int> *> right_;
  std::vector<std::vector<int> *> down_;
  std::vector<std::pair<int, int>> begin_end_;
  std::stack<QGraphicsRectItem *> solution_items_;
  std::queue<QGraphicsRectItem *> point_items_;
  void DrawSolutionLines(QGraphicsRectItem& qgritem, QPen& pen,
                                     QGraphicsScene& scene, bool to_push);
  int color_{};
  void MazeDraw(QGraphicsScene *scene);
  void DrawMazeSolution(std::vector<std::vector<int>> v);
  bool IsCorrectPoints();
  void ClearSolution();
  void ClearPoints();
};
#endif  // MAINWINDOW_H
