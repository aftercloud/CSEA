#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QScrollArea>
#include <QLabel>
#include <QMouseEvent>
#include <QVector>
#include <QFont>
#include <QPointF>
#include <QStringList>

#include "bgthread.h"
#include "timethread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void about();
    void gameStart();
    void setDegreeBasic();
    void setDegreeMedium();
    void setDegreeHard();
    void setTime(int value);
    void mousePressEvent(QMouseEvent*e);

private:
    Ui::MainWindow *ui;
    QImage Real_Image;
    QImage Virtual_Image;
    QScrollArea*Work_Area;
    QImage Game;
    QImage Copy;
    QImage*before;
    QImage*after;
    QLabel*labels;
    QVector<QVector<int>>v_mark;
    int degree;
    int height;
    int width;
    QVector<QVector<bool >>b_mark;
    int*i_mark;
    QFont font;
    QPointF pof;
    bool protected_mark;
    int uConunt;
    QStringList record;
    TimeThread time;
    BGthread bgm;

private:
    void drawBlackLine();
    void splitImage();
    void floodfill(int numy, int numx);

};

#endif // MAINWINDOW_H
