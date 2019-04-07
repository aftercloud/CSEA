#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"stack.h"

#include <QMainWindow>
#include <QFile>
#include <QLabel>
#include<QMouseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void set_reg();//设置reg的值
    void dec_to_hex(int change,QString&s);//将十进制整型数转化为十六进制字符串
    void mousePressEvent(QMouseEvent*event);
    //指令
    void addu(int a,int b,int c);//0
    void addiu(int a,int b,int c);//1
    void subu(int a,int b,int c);//2
    void sll(int a,int b,int c);//3
    void srl(int a,int b,int c);//4
    void andand(int a,int b,int c);//5
    void andi(int a,int b,int c);//6
    void oror(int a,int b,int c);//7
    void ori(int a,int b,int c);//8
    void lui(int a,int b,int c);//9
    void slt(int a,int b,int c);//10
    void beq(int a,int b,int c);//11
    void bne(int a,int b,int c);//12
    void j(int a,int b,int c);//13
    void jal(int a,int b,int c);//14
    void jr(int a,int b,int c);//15
    void lw(int a,int b,int c);//16
    void sw(int a,int b,int c);//17
    void addi(int a,int b,int c);//18
    void sdc2(int a,int b,int c);//19
    void lb(int a,int b,int c);//20
    void swc1(int a,int b,int c);//21
    void swc2(int a,int b,int c);//22
    void slti(int a,int b,int c);//23
    void mthi(int a,int b,int c);//24
    void bgtz(int a,int b,int c);//25
    void blez(int a,int b,int c);//26
    //解码函数
    void translate();
    //指令执行函数
    void conquer();
    ~MainWindow();

private slots:
    void on_menu_clicked();
    void on_menu_close();
    void show_pc();

private:
    Ui::MainWindow *ui;
    QFile file;
    char*dump;
    int reg[32];
    QLabel*lab[64];
    Stack<int>sp;
    int op;int rs;int rt;int rd;int shamt;int func;int address;int immediate;
    int start;int end;int count;
    unsigned char store[80000];int hi;int lo;
    int pc;int res;
};

#endif // MAINWINDOW_H
