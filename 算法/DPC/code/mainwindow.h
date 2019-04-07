#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include"cthread.h"
#include"dthread.h"

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
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void set_value(int value);

private:
    Ui::MainWindow *ui;

    CThread ct;

    DThread dt;
};

#endif // MAINWINDOW_H
