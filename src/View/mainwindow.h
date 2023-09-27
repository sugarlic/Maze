#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include "../Contoler/Controler.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, s21::Maze *maze = nullptr);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent* event) override;

private slots:
    void on_pushButton_clicked();

    void on_ReadMaze_clicked();

private:
    Ui::MainWindow *ui;
    s21::Controler controler_;
    std::vector<std::vector<int>*> right_;
    std::vector<std::vector<int>*> down_;
    std::vector<std::pair<int,int>> begin_end_;
    int point_count_{};
    void MazeDraw(QGraphicsScene* scene);

};
#endif // MAINWINDOW_H
