#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFile>
#include<QTextStream>
#include<QPen>
#include<QPainter>
#include<QColor>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),pos1(0),pos2(0),
    ui(new Ui::MainWindow),time(1),
    poi1(0),poi2(0),first(false),l()
{
    ui->setupUi(this);
    ui->centralWidget->setMouseTracking(true);
    ui->scrollAreaWidgetContents->setMouseTracking(true);
    ui->scrollArea->setMouseTracking(true);
    QImage*img=new QImage;
    img->load("img/-1.png");
    ui->label_6->setPixmap(QPixmap::fromImage(*img));
    img->load("img/-2.png");
    ui->label_5->setPixmap(QPixmap::fromImage(*img));
    img->load("img/标定.tif");
    int w=img->width();
    int h=img->height();
    CUG=new QImage;
    *CUG=img->scaled(w/4,h/4,Qt::KeepAspectRatio);
    ui->label_10->resize(CUG->width(),CUG->height());
    ui->label_10->setPixmap(QPixmap::fromImage(*CUG));
    delete img;
    int m=0,n=0;//点数,边数
    char*ouct;//顶点
    int*a=0,*b=0,*c=0;//端点,权值
    QFile file("data/guide.cug");
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    in>>m;
    ouct=new char[m];
    for(int i=0;i<m;i++)
        in>>ouct[i]>>ouct[i];
    in>>n;
    a=new int[n];
    b=new int[n];
    c=new int[n];
    for(int i=0;i<n;i++)
        in>>a[i]>>b[i]>>c[i];
    l.Reset(m,n,ouct,a,b,c);
    delete[]ouct;
    delete[]a;
    delete[]b;
    delete[]c;
    file.close();
    file.setFileName("data/locat.cug");
    file.open(QIODevice::ReadOnly);
    in>>n;edge=n;
    xx=new int[n];
    yy=new int[n];
    draw=new int[n];
    for(int i=0;i<edge;i++)
        draw[i]=-1;
    for(int i=0;i<n;i++)
        in>>xx[i]>>yy[i];
    file.close();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete CUG;
    delete[]xx;
    delete[]yy;
    delete[]draw;
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    ui->label_10->setGeometry(0,0,ui->label_10->width(),ui->label_10->height());
    ui->horizontalSlider_2->setSliderPosition(0);pos1=0;
    ui->horizontalSlider_3->setSliderPosition(0);pos2=0;
    QImage*img=new QImage;
    int w=CUG->width();
    int h=CUG->height();
    time=1+(double)position/100;
    *img=CUG->scaled(time*w,time*h,Qt::KeepAspectRatio);
    ui->label_10->resize(img->width(),img->height());
    ui->label_10->setPixmap(QPixmap::fromImage(*img));
    delete img;
}

void MainWindow::on_horizontalSlider_2_sliderMoved(int position)
{
    pos1=position;
    ui->label_10->setGeometry(-(8*(time-1)*position),ui->label_10->y(),ui->label_10->width(),ui->label_10->height());
}

void MainWindow::on_horizontalSlider_3_sliderMoved(int position)
{
    pos2=position;
    ui->label_10->setGeometry(ui->label_10->x(),-(8*(time-1)*position),ui->label_10->width(),ui->label_10->height());
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    int x=event->x(),y=event->y();
    if(x>=10&&y>=10&&x<=810&&y<=760)
    {
        x+=8*(time-1)*pos1;
        y+=8*(time-1)*pos2;
        x/=time;y/=time;//缩放滚动后坐标转换
        if(x>=277&&y>=521&&x<=337&&y<=595)//0-图书馆
        {
            QImage*img=new QImage;
            img->load("img/00.png");
            ui->label_5->setPixmap(QPixmap::fromImage(*img));
            img->load("img/01.png");
            ui->label_6->setPixmap(QPixmap::fromImage(*img));
            delete img;
        }

        else if(x>=286&&y>=468&&x<=359&&y<=497)//1-物探楼
        {
            QImage*img=new QImage;
            img->load("img/10.png");
            ui->label_5->setPixmap(QPixmap::fromImage(*img));
            img->load("img/11.png");
            ui->label_6->setPixmap(QPixmap::fromImage(*img));
            delete img;
        }
        else if(x>=383&&y>=451&&x<=469&&y<=497)//2-工程楼
        {
            QImage*img=new QImage;
            img->load("img/20.png");
            ui->label_5->setPixmap(QPixmap::fromImage(*img));
            img->load("img/21.png");
            ui->label_6->setPixmap(QPixmap::fromImage(*img));
            delete img;
        }
        else if(x>=394&&y>=361&&x<=463&&y<=394)//3-教一楼
        {
            QImage*img=new QImage;
            img->load("img/30.png");
            ui->label_5->setPixmap(QPixmap::fromImage(*img));
            img->load("img/31.png");
            ui->label_6->setPixmap(QPixmap::fromImage(*img));
            delete img;
        }
        else if(x>=283&&y>=358&&x<=353&&y<=396)//4-数理楼
        {
            QImage*img=new QImage;
            img->load("img/40.png");
            ui->label_5->setPixmap(QPixmap::fromImage(*img));
            img->load("img/41.png");
            ui->label_6->setPixmap(QPixmap::fromImage(*img));
            delete img;
        }
        else if(x>=458&&y>=274&&x<=486&&y<=327)//5-网络中心
        {
            QImage*img=new QImage;
            img->load("img/50.png");
            ui->label_5->setPixmap(QPixmap::fromImage(*img));
            img->load("img/51.png");
            ui->label_6->setPixmap(QPixmap::fromImage(*img));
            delete img;
        }
        else if(x>=298&&y>=285&&x<=440&&y<=303)//6-主楼
        {
            QImage*img=new QImage;
            img->load("img/60.png");
            ui->label_5->setPixmap(QPixmap::fromImage(*img));
            img->load("img/61.png");
            ui->label_6->setPixmap(QPixmap::fromImage(*img));
            delete img;
        }
        else if(x>=63&&y>=405&&x<=115&&y<=457)//7-西三学园
        {
            QImage*img=new QImage;
            img->load("img/70.png");
            ui->label_5->setPixmap(QPixmap::fromImage(*img));
            img->load("img/71.png");
            ui->label_6->setPixmap(QPixmap::fromImage(*img));
            delete img;
        }
        else if(x>=95&&y>=317&&x<=121&&y<=381)//8-弘毅堂
        {
            QImage*img=new QImage;
            img->load("img/80.png");
            ui->label_5->setPixmap(QPixmap::fromImage(*img));
            img->load("img/81.png");
            ui->label_6->setPixmap(QPixmap::fromImage(*img));
            delete img;
        }
        else if(x>=64&&y>=197&&x<=134&&y<=237)//9-公主楼
        {
            QImage*img=new QImage;
            img->load("img/90.png");
            ui->label_5->setPixmap(QPixmap::fromImage(*img));
            img->load("img/91.png");
            ui->label_6->setPixmap(QPixmap::fromImage(*img));
            delete img;
        }
        else if(x>=157&&y>=157&&x<=255&&y<=225)//10-体育馆
        {
            QImage*img=new QImage;
            img->load("img/100.png");
            ui->label_5->setPixmap(QPixmap::fromImage(*img));
            img->load("img/101.png");
            ui->label_6->setPixmap(QPixmap::fromImage(*img));
            delete img;
        }
        else//地大
        {
            QImage*img=new QImage;
            img->load("img/-2.png");
            ui->label_5->setPixmap(QPixmap::fromImage(*img));
            img->load("img/-1.png");
            ui->label_6->setPixmap(QPixmap::fromImage(*img));
            delete img;
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    int x=event->x(),y=event->y();
    if(x>=10&&y>=10&&x<=810&&y<=760)
    {
        if(first==true)//设置目的地
        {
            x+=8*(time-1)*pos1;
            y+=8*(time-1)*pos2;
            x/=time;y/=time;//缩放滚动后坐标转换
            if(x>=277&&y>=521&&x<=337&&y<=595)//0-图书馆
            {
                poi2=0;
                ui->label_4->setText("图书馆");
            }

            else if(x>=286&&y>=468&&x<=359&&y<=497)//1-物探楼
            {
                poi2=2;
                ui->label_4->setText("物探楼");
            }
            else if(x>=383&&y>=451&&x<=469&&y<=497)//2-工程楼
            {
                poi2=1;
                ui->label_4->setText("工程楼");
            }
            else if(x>=394&&y>=361&&x<=463&&y<=394)//3-教一楼
            {
                poi2=3;
                ui->label_4->setText("教一楼");
            }
            else if(x>=283&&y>=358&&x<=353&&y<=396)//4-数理楼
            {
                poi2=4;
                ui->label_4->setText("数理楼");
            }
            else if(x>=458&&y>=274&&x<=486&&y<=327)//5-网络中心
            {
                poi2=5;
                ui->label_4->setText("网络中心");
            }
            else if(x>=298&&y>=285&&x<=440&&y<=303)//6-主楼
            {
                poi2=6;
                ui->label_4->setText("主楼");
            }
            else if(x>=63&&y>=405&&x<=115&&y<=457)//7-西三学园
            {
                poi2=7;
                ui->label_4->setText("西三学园");
            }
            else if(x>=95&&y>=317&&x<=121&&y<=381)//8-弘毅堂
            {
                poi2=8;
                ui->label_4->setText("弘毅堂");
            }
            else if(x>=64&&y>=197&&x<=134&&y<=237)//9-公主楼
            {
                poi2=9;
                ui->label_4->setText("公主楼");
            }
            else if(x>=157&&y>=157&&x<=255&&y<=225)//10-体育馆
            {
                poi2=10;
                ui->label_4->setText("体育馆");
            }
            else//地大
            {
                return;
            }
            first=false;
            QPen pen(QColor(255,1,2));
            pen.setWidth(5);
            QPainter painter(CUG);
            painter.setPen(pen);
            painter.setRenderHint(QPainter::Antialiasing, true);
            QString s="";
            l.Di(poi1,poi2,s,edge);
            for(int h=0;h<s.length();h++)
                draw[h]=s[h].toLatin1()-65;
            int x1,x2,y1,y2;
            int i=0;
            while(i<=edge-2&&draw[i+1]!=-1){
                x1=xx[draw[i]];
                y1=yy[draw[i]];
                x2=xx[draw[i+1]];
                y2=yy[draw[++i]];
            painter.drawLine(x1,y1,x2,y2);
            }
            for(i=0;i<edge;i++)
                draw[i]=-1;
            QImage*img=new QImage;
            int w=CUG->width();
            int h=CUG->height();
            *img=CUG->scaled(time*w,time*h,Qt::KeepAspectRatio);
            ui->label_10->resize(img->width(),img->height());
            ui->label_10->setPixmap(QPixmap::fromImage(*img));
            delete img;
        }
        else//设置起始点
        {
            x+=8*(time-1)*pos1;
            y+=8*(time-1)*pos2;
            x/=time;y/=time;//缩放滚动后坐标转换
            if(x>=277&&y>=521&&x<=337&&y<=595)//0-图书馆
            {
                poi1=0;
                ui->label_2->setText("图书馆");
            }

            else if(x>=286&&y>=468&&x<=359&&y<=497)//1-物探楼
            {
                poi1=2;
                ui->label_2->setText("物探楼");
            }
            else if(x>=383&&y>=451&&x<=469&&y<=497)//2-工程楼
            {
                poi1=1;
                ui->label_2->setText("工程楼");
            }
            else if(x>=394&&y>=361&&x<=463&&y<=394)//3-教一楼
            {
                poi1=3;
                ui->label_2->setText("教一楼");
            }
            else if(x>=283&&y>=358&&x<=353&&y<=396)//4-数理楼
            {
                poi1=4;
                ui->label_2->setText("数理楼");
            }
            else if(x>=458&&y>=274&&x<=486&&y<=327)//5-网络中心
            {
                poi1=5;
                ui->label_2->setText("网络中心");
            }
            else if(x>=298&&y>=285&&x<=440&&y<=303)//6-主楼
            {
                poi1=6;
                ui->label_2->setText("主楼");
            }
            else if(x>=63&&y>=405&&x<=115&&y<=457)//7-西三学园
            {
                poi1=7;
                ui->label_2->setText("西三学园");
            }
            else if(x>=95&&y>=317&&x<=121&&y<=381)//8-弘毅堂
            {
                poi1=8;
                ui->label_2->setText("弘毅堂");
            }
            else if(x>=64&&y>=197&&x<=134&&y<=237)//9-公主楼
            {
                poi1=9;
                ui->label_2->setText("公主楼");
            }
            else if(x>=157&&y>=157&&x<=255&&y<=225)//10-体育馆
            {
                poi1=10;
                ui->label_2->setText("体育馆");
            }
            else//地大
            {
                return;
            }
            first=true;
            ui->label_4->setText("待定");
        }
    }
}
