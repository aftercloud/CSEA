#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QImage>
#include<QMouseEvent>
#include"graph.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void mouseMoveEvent(QMouseEvent*event);
    void mousePressEvent(QMouseEvent*event);
    ~MainWindow();

private slots:
    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_2_sliderMoved(int position);

    void on_horizontalSlider_3_sliderMoved(int position);

private:
    Ui::MainWindow *ui;
    QImage*CUG;//存放标定图
    double time;
    int pos1;//左右
    int pos2;//上下
    bool first;//起始点/目的地切换
    int poi1;//起始点
    int poi2;//目的地
    Graph<>l;
    int*xx;int*yy;
    int*draw;int edge;
};

#endif // MAINWINDOW_H
