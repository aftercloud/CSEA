#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<vector>
#include<QTreeWidgetItem>

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
    void create();
    void open();
    void save();
    void other();
    void copy();
    void paste();
    void undo();
    void redo();
    void empty();
    void tree();
    void close();
    void Delete();
    void Inquire();

    void on_plainTextEdit_textChanged();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    bool is_open;
    bool is_create;
    QString file_name;
    std::vector<QString>un;
    std::vector<QString>re;
    QTreeWidgetItem**forset;
    std::vector<QString>key;//键
    std::vector<QString>value;//值
    std::vector<bool>visit;//键值对
    std::vector<bool>str;//字符串
    QString sma;
};

#endif // MAINWINDOW_H
