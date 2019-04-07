#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QIcon>
#include<QBrush>
#include<QPixmap>
#include<QPainter>
#include<QPalette>
#include<QMessageBox>
#include<QRandomGenerator>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),Real_Image(),Virtual_Image(),time(),bgm(),
    Game(),Copy(),v_mark(),b_mark(),pof(15,15),font(),record(),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("GameStarts");
    this->setWindowIcon(QIcon("icon/aowu.ico"));
    Work_Area = 0;labels = 0;
    before = 0;after = 0;i_mark = 0;
    font.setFamily("Microsoft YaHei");
    font.setPointSize(16);
    Real_Image.load("image/blue.png");
    Real_Image = Real_Image.scaled(1280, 720, Qt::KeepAspectRatio);
    Virtual_Image.load("image/white.png");
    Virtual_Image = Virtual_Image.scaled(1280, 720, Qt::KeepAspectRatio);

    //背景音乐
    bgm.start();

    //判定保护
    protected_mark = true;

    //难度初始化
    degree = 0;
    height = 10;
    width = 20;
    uConunt = 0;

    //背景图片加载
    {
        QPixmap pix = QPixmap::fromImage(Real_Image);
        QPalette pale;
        pale.setBrush(this->backgroundRole(), QBrush(pix));
        this->setPalette(pale);
    }

    //设置字体颜色
    {
        QPalette pale;
        pale.setColor(QPalette::WindowText, Qt::white);
        ui->label->setPalette(pale);
        ui->label_2->setPalette(pale);
        ui->label_3->setPalette(pale);
        ui->label_4->setPalette(pale);
    }

    //绑定
    {
        connect(ui->actionStart, SIGNAL(triggered()), this, SLOT(gameStart()));
        connect(ui->actionBasic, SIGNAL(triggered()), this, SLOT(setDegreeBasic()));
        connect(ui->actionmedium, SIGNAL(triggered()), this, SLOT(setDegreeMedium()));
        connect(ui->actionHard, SIGNAL(triggered()), this, SLOT(setDegreeHard()));
        connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
        connect(&time, SIGNAL(value(int)), this, SLOT(setTime(int)));
    }

}

MainWindow::~MainWindow()
{
    delete ui;
    if(before != 0)
        delete[] before;
    if(after != 0)
        delete[] after;
    if(i_mark != 0)
        delete[] i_mark;
}

void MainWindow::about()
{
    QMessageBox::information(this, "Tip", "yeudiko@foxmail.com");
}

void MainWindow::setDegreeBasic(){
    //判定保护
    protected_mark = true;

    //难度初始化
    degree = 0;
    height = 10;
    width = 20;
    ui->label_3->setText("Basic");

    //容器初始化
    if(Work_Area != 0)
        delete Work_Area;
    Work_Area = new QScrollArea(this);
    Work_Area->setGeometry(40, 100, 600, 300);
    Work_Area->show();

    //截取原始图片
    Game = Real_Image.copy(40, 100, 600, 300);
    Copy = Virtual_Image.copy(40, 100, 600, 300);
    this->drawBlackLine();

    //切割图片
    if(before != 0)
        delete[] before;
    if(after != 0)
        delete[] after;
    this->splitImage();

}

void MainWindow::setDegreeMedium(){
    //判定保护
    protected_mark = true;

    //难度初始化
    degree = 1;
    height = 15;
    width = 30;
    ui->label_3->setText("Medium");

    //容器初始化
    if(Work_Area != 0)
        delete Work_Area;
    Work_Area = new QScrollArea(this);
    Work_Area->setGeometry(40, 100, 900, 450);
    Work_Area->show();

    //截取原始图片
    Game = Real_Image.copy(40, 100, 900, 450);
    Copy = Virtual_Image.copy(40, 100, 900, 450);
    this->drawBlackLine();

    //切割图片
    if(before != 0)
        delete[] before;
    if(after != 0)
        delete[] after;
    this->splitImage();

}

void MainWindow::setDegreeHard(){
    //判定保护
    protected_mark = true;

    //难度初始化
    degree = 2;
    height = 20;
    width = 40;
    ui->label_3->setText("Hard");

    //容器初始化
    if(Work_Area != 0)
        delete Work_Area;
    Work_Area = new QScrollArea(this);
    Work_Area->setGeometry(40, 100, 1200, 600);
    Work_Area->show();

    //截取原始图片
    Game = Real_Image.copy(40, 100, 1200, 600);
    Copy = Virtual_Image.copy(40, 100, 1200, 600);
    this->drawBlackLine();

    //切割图片
    if(before != 0)
        delete[] before;
    if(after != 0)
        delete[] after;
    this->splitImage();

}

void MainWindow::drawBlackLine(){
    QPainter painter(&Copy);
    switch (this->degree) {
    //初级
    case 0:
        for(int i=1;i<height;i++){
            painter.drawLine(0, i*30, 600, i*30);
        }
        for(int i=1;i<width;i++){
            painter.drawLine(i*30, 0, i*30, 300);
        }
        break;
        //中级
    case 1:
        for(int i=1;i<height;i++){
            painter.drawLine(0, i*30, 900, i*30);
        }
        for(int i=1;i<width;i++){
            painter.drawLine(i*30, 0, i*30, 450);
        }
        break;
        //高级
    default:
        for(int i=1;i<height;i++){
            painter.drawLine(0, i*30, 1200, i*30);
        }
        for(int i=1;i<width;i++){
            painter.drawLine(i*30, 0, i*30, 600);
        }
        break;
    }
}

void MainWindow::splitImage()
{
    labels = new QLabel[height*width];
    before = new QImage[height*width];
    after = new QImage[height*width];
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            before[i*width+j] = Copy.copy(j*30, i*30, 30, 30);
            after[i*width+j] = Game.copy(j*30, i*30, 30, 30);
            labels[i*width+j].setParent(Work_Area);
            labels[i*width+j].setGeometry(j*30, i*30, 30, 30);
            labels[i*width+j].setPixmap(QPixmap::fromImage(before[i*width+j]));
            labels[i*width+j].show();
        }
    }
}

void MainWindow::gameStart()
{
    if(Work_Area == 0)
    {
        QMessageBox::information(this, "Tip", "Please choose the degree.");
    }
    else
    {
        uConunt = 0;
        b_mark.clear();
        v_mark.clear();
        record.clear();
        time.start();
        ui->label_4->setText("0");
        //初始化棋盘数据
        {
            if(i_mark != 0)
                delete[] i_mark;
            i_mark = new int[height*width];
            for(int i=0;i<height;i++){
                QVector<bool>tmp(width, false);
                for(int j=0;j<width;j++){
                    i_mark[i*width+j] = 0;
                }
                b_mark.append(tmp);
            }
        }
        //随机生成地雷
        {
            int chance = degree*5 + 5;
            for(int i=0;i<height;i++){
                for(int j=0;j<width;j++){
                    int judge = QRandomGenerator::global()->bounded(100);
                    if(judge<=chance){
                        i_mark[i*width+j] = -1;
                        uConunt++;
                    }
                }
            }
        }
        //建立地雷表
        {
            for(int i=0;i<height;i++){
                QVector<int>tmp;
                for(int j=0;j<width;j++){
                    if(i_mark[i*width+j] == -1){
                        tmp.append(-1);
                        continue;
                    }
                    int count = 0;
                    //左
                    if(j>0&&i_mark[i*width+j-1] == -1)
                        count++;
                    //右
                    if(j<width-1&&i_mark[i*width+j+1] == -1)
                        count++;
                    //上
                    if(i>0&&i_mark[i*width+j-width] == -1)
                        count++;
                    //下
                    if(i<height-1&&i_mark[i*width+j+width] == -1)
                        count++;
                    //左上
                    if(i>0&&j>0&&i_mark[i*width+j-1-width] == -1)
                        count++;
                    //右下
                    if(i<height-1&&j<width-1&&i_mark[i*width+j+1+width] == -1)
                        count++;
                    //左下
                    if(i<height-1&&j>0&&i_mark[i*width+j-1+width] == -1)
                        count++;
                    //右上
                    if(i>0&&j<width-1&&i_mark[i*width+j+1-width] == -1)
                        count++;
                    tmp.append(count);
                }
                v_mark.append(tmp);
            }
        }
        protected_mark = false;
    }
 }

void MainWindow::mousePressEvent(QMouseEvent *e){
    //左键点击
    if(protected_mark)return;
    if(e->button() & Qt::LeftButton){
        int x = e->x() - 40;
        int y = e->y() - 100;
        if(x >= 0 && y >= 0 && x < 30*width && y < 30*height){
            int numx = x/30;
            int numy = y/30;
            if(v_mark[numy][numx] == -1){
                for(int i=0;i<height;i++){
                    for(int j=0;j<width;j++){
                        if(v_mark[i][j] == -1){
                            int index = i*width+j;
                            QImage itp = labels[index].pixmap()->toImage();
                            QPainter painter(&itp);
                            painter.setPen(Qt::red);
                            painter.setFont(font);
                            painter.drawText(pof, "V");
                            labels[index].setPixmap(QPixmap::fromImage(itp));
                        }
                    }
                }
                time.set();
                QMessageBox::information(this, "Tip", "Failed.");
                protected_mark = true;
                return;
            }
            if(!b_mark[numy][numx])
            {
                floodfill(numy, numx);
                for(int i = 0;i < height;i++)
                    for(int j = 0;j < width;j++)
                        if(b_mark[i][j]){
                            if(v_mark[i][j] == 0)
                                labels[i*width+j].setPixmap(QPixmap::fromImage(after[i*width+j]));
                            else{
                                 QImage itp = after[i*width+j].copy();
                                 QPainter painter(&itp);
                                 painter.setFont(font);
                                 painter.drawText(pof, QString::number(v_mark[i][j]));
                                 labels[i*width+j].setPixmap(QPixmap::fromImage(itp));
                            }
                        }
                int cnt = 0;
                for(int i=0;i<height;i++){
                    for(int j=0;j<width;j++){
                        if(!b_mark[i][j])cnt++;
                    }
                }
                if(cnt == uConunt){
                    time.set();
                    QMessageBox::information(this, "Tip", "Win.");
                    protected_mark = true;
                }
            }
        }
    }
    //右键点击
    else{
        int x = e->x() - 40;
        int y = e->y() - 100;
        if(x >= 0 && y >= 0 && x < 30*width && y < 30*height){
            int numx = x/30;
            int numy = y/30;
            int index = numy*width+numx;
            if(b_mark[numy][numx])return;
            if(!record.contains(QString::number(index))){
                record<<QString::number(index);
                QImage itp = labels[index].pixmap()->toImage();
                QPainter painter(&itp);
                painter.setPen(Qt::red);
                painter.setFont(font);
                painter.drawText(pof, "V");
                labels[index].setPixmap(QPixmap::fromImage(itp));
            }
            else{
                record.removeOne(QString::number(index));
                labels[index].setPixmap(QPixmap::fromImage(before[index]));
            }
        }
    }
}

void MainWindow::floodfill(int numy, int numx)
{
    if(b_mark[numy][numx])return;
    if(v_mark[numy][numx] > 0){
        b_mark[numy][numx] = true;
        return;
    }
    else{
        b_mark[numy][numx] = true;
        if(numy > 0)
            floodfill(numy - 1, numx);
        if(numy < height - 1)
            floodfill(numy + 1, numx);
        if(numx > 0)
            floodfill(numy, numx - 1);
        if(numx < width - 1)
            floodfill(numy, numx + 1);
    }
}

void MainWindow::setTime(int value)
{
    QString s = QString::number(value);
    ui->label_4->setText(s);
}
