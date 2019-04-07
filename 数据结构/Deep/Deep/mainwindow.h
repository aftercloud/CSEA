#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include"dthread.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void ghost();
    void display();
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void load_file();
    void load_index();
    void add_index();
    void set_analyse(int an);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
    DThread dt;
    bool is_first;
    bool is_write;
    int file_size;
    QString _size;
    QStringList ql;
    int page_num;
};

#endif // MAINWINDOW_H
