#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFile>
#include<QFileDialog>
#include<QDataStream>
#include<QVector>
#include<QIcon>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),ct(),dt(),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("DPC");
    this->setWindowIcon(QIcon("icon/aowu.ico"));
    connect(&ct,SIGNAL(value(int)),this,SLOT(set_value(int)));
    connect(&dt,SIGNAL(value(int)),this,SLOT(set_value(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName();
    ui->lineEdit->setText(file_name);
    ui->progressBar->setValue(0);
    if(file_name.endsWith(".bmp"))
    {
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(false);
    }
    else if(file_name.endsWith(".huf"))
    {
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(true);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->pushButton_2->setEnabled(false);
    ui->progressBar->setValue(0);
    QString open_name = ui->lineEdit->text();
    ct.set(open_name);
    ct.start();
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->pushButton_3->setEnabled(false);
    ui->progressBar->setValue(0);
    QString open_name = ui->lineEdit->text();
    dt.set(open_name);
    dt.start();
}

void MainWindow::set_value(int value)
{
    ui->progressBar->setValue(value);
}
