#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QUrl>
#include<QIcon>
#include<QRegExp>
#include<QRegExpValidator>
#include<QFileDialog>
#include<QDesktopServices>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),dt(),ql(),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("icon/aowu.ico"));
    this->setWindowTitle("Deep");
    ui->plainTextEdit->setReadOnly(true);page_num=1;
    ui->lineEdit->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9' ]{100}"),this));
    connect(ui->action,SIGNAL(triggered()),this,SLOT(load_file()));
    connect(ui->action_2,SIGNAL(triggered()),this,SLOT(load_index()));
    connect(ui->action_4,SIGNAL(triggered()),this,SLOT(add_index()));
    connect(&dt,SIGNAL(analyse(int)),this,SLOT(set_analyse(int)));
    _size="";this->ghost();is_first=true;file_size=0;is_write=false;
    ui->label->setVisible(false);ui->label_2->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString s=ui->lineEdit->text();
    if(s.size()>0)
    {
        ui->label->setVisible(true);
        ui->label_2->setVisible(true);
        dt.inds();
        dt.select(-3);
        dt.search(s);
        dt.start();
    }
}

void MainWindow::load_file()
{
    this->ghost();
    ui->label->setVisible(true);
    ui->label_2->setVisible(true);
    dt.select(0);
    dt.set(QFileDialog::getOpenFileName(NULL,"Deep",".","*.txt"));
    dt.start();
}

void MainWindow::load_index()
{
    this->ghost();
    ui->label->setVisible(true);
    ui->label_2->setVisible(true);
    dt.select(-1);
    dt.set(QFileDialog::getOpenFileName(NULL,"Deep",".","*.txt"));
    dt.start();
}

void MainWindow::add_index()
{
    this->ghost();
    ui->label->setVisible(true);
    ui->label_2->setVisible(true);
    dt.select(-2);
    dt.set(QFileDialog::getOpenFileName(NULL,"Deep",".","*.txt"));
    dt.start();
}

void MainWindow::ghost()
{
    ui->pushButton_2->setVisible(false);
    ui->pushButton_3->setVisible(false);
    ui->pushButton_4->setVisible(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
}

void MainWindow::display()
{
    ui->pushButton_2->setVisible(true);
    ui->pushButton_3->setVisible(true);
    ui->pushButton_4->setVisible(true);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_4->setEnabled(true);
}

void MainWindow::set_analyse(int an)
{
    if(an<0)
    {
        is_first=true;
        is_write=false;
        ui->label->setText("索引");
        ui->label_2->setText("加载完成");
        ui->pushButton_4->setVisible(true);
        ui->pushButton_4->setEnabled(true);
        ql=dt.get_result();
        QString s="";
        if(ql.size()==0)
        {
            s="认为 可信度大于等于2 的网页即为可信网页\r\n";
            s+="索引整体无可信网页\r\n";
            s+="对于搜索网页将按可信度从高到低排序\r\n";
            s+="请勿点击 获取信息 按钮";
        }
        else
        {
            for(int i=0;i<ql.size();i++)
            {
                s+=ql.at(i);
                s+="\r\n";
            }
        }
        ui->plainTextEdit->setPlainText(s);
        return;
    }
    if(an==2146000000)
    {
        QDesktopServices::openUrl(QUrl("show.txt"));
        return;
    }
    if(an==2147000000)
    {
        this->display();
        page_num=1;
        ui->label->setText("网页数");
        QString s="";
        ql=dt.get_result();
        if(ql.size()>10)
        {
            s="10/"+QString::number(ql.size());
            ui->label_2->setText(s);s="";
            for(int i=0;i<10;i++)
            {
                s+=ql.at(i);
                s+="\r\n";
            }
        }
        else
        {
            s=QString::number(ql.size())+"/"+QString::number(ql.size());
            ui->label_2->setText(s);s="";
            for(int i=0;i<ql.size();i++)
            {
                s+=ql.at(i);
                s+="\r\n";
            }
        }
        ui->plainTextEdit->setPlainText(s);
        return;
    }
    if(is_first&&!is_write)
    {
        is_first=false;
        file_size=an;
        _size="/"+QString::number(an)+"  字节";
        ui->label->setText("解析文件");
        return;
    }
    QString s=QString::number(an)+_size;
    ui->label_2->setText(s);
    if(is_first&&is_write)
    {
        is_first=false;
        ui->label->setText("写入索引");
        file_size=an;
        _size="/"+QString::number(an)+"  条目";
        return;
    }
    if(an==file_size)
    {
        if(!is_write)
        {
            is_first=true;
            is_write=true;
        }
        else
        {
            is_first=true;
            is_write=false;
        }
    }
}

void MainWindow::on_pushButton_2_clicked()//上一页
{
    if(page_num<2)return;
    QString s="";page_num--;
    s=QString::number(page_num*10)+"/"+QString::number(ql.size());
    ui->label_2->setText(s);s="";
    for(int i=(page_num-1)*10;i<page_num*10;i++)
    {
        s+=ql.at(i);
        s+="\r\n";
    }
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::on_pushButton_3_clicked()//下一页
{
    page_num++;
    if((page_num-1)*10>ql.size())
    {
        page_num--;
        return;
    }
    QString s="";
    int range=page_num*10>ql.size()?ql.size():page_num*10;
    s=QString::number(range)+"/"+QString::number(ql.size());
    ui->label_2->setText(s);s="";
    for(int i=(page_num-1)*10;i<range;i++)
    {
        s+=ql.at(i);
        s+="\r\n";
    }
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::on_pushButton_4_clicked()//获取内容
{
    dt.select(page_num);
    dt.start();
}
