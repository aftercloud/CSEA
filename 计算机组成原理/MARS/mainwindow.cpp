#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFile>
#include<QDataStream>
#include<QFileDialog>

MainWindow::MainWindow(QWidget *parent) :hi(0),lo(0),
    QMainWindow(parent),file(),sp(),start(1),end(1),count(0),pc(0),res(0),
    op(0),rs(0),rt(0),rd(0),shamt(0),func(0),address(0),immediate(0),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);ui->centralWidget->setMouseTracking(true);
    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(on_menu_clicked()));
    connect(ui->actionClose,SIGNAL(triggered()),this,SLOT(on_menu_close()));
    lab[0]=ui->label_33;
    lab[1]=ui->label_34;
    lab[2]=ui->label_35;
    lab[3]=ui->label_36;
    lab[4]=ui->label_37;
    lab[5]=ui->label_38;
    lab[6]=ui->label_39;
    lab[7]=ui->label_40;
    lab[8]=ui->label_41;
    lab[9]=ui->label_42;
    lab[10]=ui->label_43;
    lab[11]=ui->label_44;
    lab[12]=ui->label_45;
    lab[13]=ui->label_46;
    lab[14]=ui->label_47;
    lab[15]=ui->label_48;
    lab[16]=ui->label_49;
    lab[17]=ui->label_50;
    lab[18]=ui->label_51;
    lab[19]=ui->label_52;
    lab[20]=ui->label_53;
    lab[21]=ui->label_54;
    lab[22]=ui->label_55;
    lab[23]=ui->label_56;
    lab[24]=ui->label_57;
    lab[25]=ui->label_58;
    lab[26]=ui->label_59;
    lab[27]=ui->label_60;
    lab[28]=ui->label_61;
    lab[29]=ui->label_62;
    lab[30]=ui->label_63;
    lab[31]=ui->label_64;
    lab[32]=ui->label;
    lab[33]=ui->label_2;
    lab[34]=ui->label_3;
    lab[35]=ui->label_4;
    lab[36]=ui->label_5;
    lab[37]=ui->label_6;
    lab[38]=ui->label_7;
    lab[39]=ui->label_8;
    lab[40]=ui->label_9;
    lab[41]=ui->label_10;
    lab[42]=ui->label_11;
    lab[43]=ui->label_12;
    lab[44]=ui->label_13;
    lab[45]=ui->label_14;
    lab[46]=ui->label_15;
    lab[47]=ui->label_16;
    lab[48]=ui->label_17;
    lab[49]=ui->label_18;
    lab[50]=ui->label_19;
    lab[51]=ui->label_20;
    lab[52]=ui->label_21;
    lab[53]=ui->label_22;
    lab[54]=ui->label_23;
    lab[55]=ui->label_24;
    lab[56]=ui->label_25;
    lab[57]=ui->label_26;
    lab[58]=ui->label_27;
    lab[59]=ui->label_28;
    lab[60]=ui->label_29;
    lab[61]=ui->label_30;
    lab[62]=ui->label_31;
    lab[63]=ui->label_32;
    for(int i=0;i<80000;i++)
        store[i]=0;
    for(int i=0;i<32;i++)
        reg[i]=0;
    reg[28]=268468224;
    reg[29]=4210688;
    dump=new char[4];//指令
}

MainWindow::~MainWindow()
{
    delete ui;
    delete[]dump;
}

void MainWindow::on_menu_clicked()
{
    QString s=QFileDialog::getOpenFileName();
    file.setFileName(s);
    file.open(QIODevice::ReadOnly);
    end=file.size()/4;
    QString show="文件 "+s+" 读取成功\r\n在文本框外任意位置单击鼠标开始单步跟踪\r\n";
    ui->plainTextEdit->setPlainText(show);show_pc();
}

void MainWindow::on_menu_close()
{
    file.close();
    ui->plainTextEdit->clear();
    for(int i=0;i<80000;i++)
        store[i]=0;
    for(int i=0;i<32;i++)
        reg[i]=0;
    reg[28]=268468224;
    reg[29]=4210688;
    set_reg();count=0;
    start=end=1;pc=res=0;
}

void MainWindow::set_reg()
{
    QString s;reg[0]=0;
    for(int i=0;i<32;i++)
    {
        dec_to_hex(reg[i],s);
        lab[i]->setText(s);
    }
}

void MainWindow::dec_to_hex(int change, QString &s)
{
    if(change==0)
    {
        s="0x00000000";
        return;
    }
    QString s1="",s2="0x";
    Stack<int>l;
    int temp=0;//接收余数
    int sc=change;
    if(change<0)change*=-1;
    while(change!=0)
    {
        temp=change%16;
        l.Push((unsigned int)temp);
        change/=16;
    }
    while(!l.IsEmpty())
    {
        l.Pop(temp);
        if(temp<10)
        {
            s1+=QChar(temp+48);
        }
        else{
            s1+=QChar(temp+87);
        }
    }
    if(sc<0)s2+="1";
    for(int i=0;i<8-s1.length();i++)
        s2+="0";
    if(sc<0)
        s2.remove(3,1);
    s=s2+s1;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(start==end+1||end==1)return;
    start++;//向下读取指令
    QDataStream out(&file);
    out.readRawData(dump,4);//执行指令
    translate();
    conquer();
    set_reg();
    show_pc();
}

void MainWindow::translate()//将32位机器指令分解成若干部分
{
    char t1=0,t2=0,ch=31;
    op=(unsigned char)(dump[0]);
    op>>=2;
    t1=((unsigned char)dump[0])<<6;
    t2=((unsigned char)dump[1])>>5;
    t1=((unsigned char)t1)>>3;
    rs=(unsigned char)(t1|t2);
    rt=(unsigned char)(dump[1]&ch);
    rd=(unsigned char)(ch&((unsigned char)dump[2])>>3);
    t1=((unsigned char)dump[2])<<5;
    t1=((unsigned char)t1)>>3;
    t2=((unsigned char)dump[3])>>6;
    shamt=(unsigned char)(t1|t2);
    func=(unsigned char)dump[3];
    func<<=2;
    func=((unsigned char)func)>>2;
    address=(unsigned char)dump[2];
    address=address<<8;
    address=address+(unsigned char)dump[3];
    if((unsigned char)dump[2]>127)
    {
        unsigned char temp=~((unsigned char)dump[2]);
        address=temp;
        address<<=8;
        temp=~((unsigned char)dump[3]);
        address=address+temp+1;
        address*=-1;
    }
    immediate=rs;
    immediate=immediate>>3;
    immediate=immediate<<8;
    immediate=immediate|(unsigned char)dump[1];
    immediate=immediate<<8;
    immediate=immediate|(unsigned char)dump[2];
    immediate=immediate<<8;
    immediate=immediate|(unsigned char)dump[3];
}

void MainWindow::conquer()//只改变reg数组的值
{
    count++;
    if(op==0)//R型指令
    {
        if(func==55||func==21||func==47){
            QString s=ui->plainTextEdit->toPlainText();
            s=s+QString::number(count)+" 不存在 opcode="+QString::number(op)+" funct="+QString::number(func)+" 的指令\r\n";
            ui->plainTextEdit->setPlainText(s);
        }
        else if(func==0)
            sll(rd,rs,shamt);
        else if(func==8)
            jr(0,0,0);
        else if(func==2)
            srl(rd,rs,shamt);
        else if(func==17)
            mthi(rd,0,0);
        else if(func==33)
            addu(rd,rs,rd);
        else if(func==35)
            subu(rd,rs,rt);
        else if(func==36)
            andand(rd,rs,rt);
        else if(func==37)
            oror(rd,rs,rt);
        else if(func==42)
            slt(rd,rs,rt);
        else{
            QString s=ui->plainTextEdit->toPlainText();
            s=s+QString::number(count)+" 该R型指令无法识别\r\n";
            ui->plainTextEdit->setPlainText(s);
        }
    }
    else if(op==2)//J型指令
    {
        j(immediate,0,0);
    }
    else if(op==3)
    {
        jal(immediate,0,0);
    }
    else//I型指令
    {
        if(op==8)
            addi(rd,rs,address);
        else if(op==4)
            beq(rd,rs,address);
        else if(op==5)
            bne(rd,rs,address);
        else if(op==6)
            blez(rd,rs,address);
        else if(op==7)
            bgtz(rd,0,address);
        else if(op==9)
            addiu(rd,rs,address);
        else if(op==10)
            slti(rd,rs,address);
        else if(op==12)
            andi(rd,rs,address);
        else if(op==13)
            ori(rd,rs,address);
        else if(op==15)
            lui(rd,0,address);
        else if(op==35)
            lw(rd,rs,address);
        else if(op==43)
            sw(rd,rs,address);
        else if(op==62)
            sdc2(rd,rs,address);
        else if(op==32)
            lb(rd,rs,address);
        else if(op==57)
            swc1(rd,rs,address);
        else if(op==58)
            swc2(rd,rs,address);
        else if(op==1||op==16||op==59||op==63)
        {
            QString s=ui->plainTextEdit->toPlainText();
            s=s+QString::number(count)+" 不存在 opcode="+QString::number(op)+" 的指令\r\n";
            ui->plainTextEdit->setPlainText(s);
        }
        else{//无法识别
            QString s=ui->plainTextEdit->toPlainText();
            s=s+" 未知指令无法识别\r\n";
            ui->plainTextEdit->setPlainText(s);
        }
    }
}

void MainWindow::addu(int a,int b,int c)//0
{
    QString s=ui->plainTextEdit->toPlainText();
    if(reg[c]<0)reg[c]*=-1;
    reg[a]=reg[b]+reg[c];
    s+=QString::number(count)+" addu ";
    s=s+lab[a+32]->text()+" , "+lab[b+32]->text()+" , "+lab[b+32]->text()+"\r\n";
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::addiu(int a,int b,int c)//1
{
    QString s=ui->plainTextEdit->toPlainText();
    reg[a]=reg[b]+c;
    s+=QString::number(count)+" addiu ";
    s=s+lab[a+32]->text()+" , "+lab[b+32]->text()+" , "+QString::number(c)+"\r\n";
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::subu(int a,int b,int c)//2
{
    QString s=ui->plainTextEdit->toPlainText();
    if(reg[c]<0)reg[c]*=-1;
    reg[a]=reg[b]-reg[c];
    s+=QString::number(count)+" subu ";
    s=s+lab[a+32]->text()+" , "+lab[b+32]->text()+" , "+lab[b+32]->text()+"\r\n";
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::sll(int a,int b,int c)//3
{
    reg[a]=reg[b]<<c;
    QString s=ui->plainTextEdit->toPlainText();
    s+=QString::number(count)+" sll ";
    s=s+lab[a+32]->text()+" , "+lab[b+32]->text()+" , "+lab[b+32]->text()+"\r\n";
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::srl(int a,int b,int c)//4
{
    reg[a]=reg[b]>>c;
    QString s=ui->plainTextEdit->toPlainText();
    s+=QString::number(count)+" srl ";
    s=s+lab[a+32]->text()+" , "+lab[b+32]->text()+" , "+lab[b+32]->text()+"\r\n";
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::andand(int a,int b,int c)//5
{
    reg[a]=reg[b]&reg[c];
    QString s=ui->plainTextEdit->toPlainText();
    s+=QString::number(count)+" and ";
    s=s+lab[a+32]->text()+" , "+lab[b+32]->text()+" , "+lab[b+32]->text()+"\r\n";
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::andi(int a,int b,int c)//6
{
    reg[a]=reg[b]&c;
    QString s=ui->plainTextEdit->toPlainText();
    s+=QString::number(count)+" andi ";
    s=s+lab[a+32]->text()+" , "+lab[b+32]->text()+" , "+lab[b+32]->text()+"\r\n";
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::oror(int a,int b,int c)//7
{
    reg[a]=reg[b]|reg[c];
    QString s=ui->plainTextEdit->toPlainText();
    s+=QString::number(count)+" or ";
    s=s+lab[a+32]->text()+" , "+lab[b+32]->text()+" , "+lab[b+32]->text()+"\r\n";
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::ori(int a,int b,int c)//8
{
    reg[a]=reg[b]|c;
    QString s=ui->plainTextEdit->toPlainText();
    s+=QString::number(count)+" ori ";
    s=s+lab[a+32]->text()+" , "+lab[b+32]->text()+" , "+lab[b+32]->text()+"\r\n";
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::lui(int a,int b,int c)//9
{
    reg[a]=address;
    if(reg[a]<0)reg[a]*=-1;
    reg[a]<<=16;
    QString s=ui->plainTextEdit->toPlainText();
    s+=QString::number(count)+" lui ";
    s=s+lab[a+32]->text()+" , "+QString::number(c)+"\r\n";
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::slt(int a,int b,int c)//10
{
    if(reg[b]<reg[c])
        reg[a]=1;
    else reg[a]=0;
    QString s=ui->plainTextEdit->toPlainText();
    s+=QString::number(count)+" slti ";
    s=s+lab[a+32]->text()+" , "+lab[b+32]->text()+" , "+lab[b+32]->text()+"\r\n";
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::beq(int a,int b,int c)//11
{
    if(reg[a]==reg[b])
    {
        int temp=file.pos();
        temp=temp+c;
        file.seek(temp);
    }
    QString s=ui->plainTextEdit->toPlainText();
    s+=QString::number(count)+" bne ";
    s=s+lab[a+32]->text()+" , "+lab[b+32]->text()+" , "+QString::number(c);
    if((file.pos()+c)<0||(file.pos()+c)>end)
        s+=" # 跳转地址溢出";
    s+="\r\n";
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::bne(int a,int b,int c)//12
{
    if(reg[a]!=reg[b])
    {
        int temp=file.pos();
        temp=temp+c;
        file.seek(temp);
    }
    QString s=ui->plainTextEdit->toPlainText();
    s+=QString::number(count)+" bne ";
    s=s+lab[a+32]->text()+" , "+lab[b+32]->text()+" , "+QString::number(c);
    if((file.pos()+c)<0||(file.pos()+c)>end)
        s+=" # 跳转地址溢出";
    s+="\r\n";
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::j(int a,int b,int c)//13
{
    if(a*4>=0&&a*4<=file.size()){
        file.seek(a*4);
        start=a;
    }
    QString s=ui->plainTextEdit->toPlainText();
    s+=QString::number(count)+" j "+QString::number(a);
    if(a*4<0||a*4>file.size()){
        s+=" # 跳转地址溢出";
    }
    s+="\r\n";
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::jal(int a,int b,int c)//14
{
    if(a*4>=0&&a*4<=file.size()){
        file.seek(a*4);
        start=a;
    }
    reg[31]=file.pos()+4;
    QString s=ui->plainTextEdit->toPlainText();
    s+=QString::number(count)+" jal "+QString::number(a);
    if(a*4<0||a*4>file.size()){
        s+=" # 跳转地址溢出";
    }
    s+="\r\n";
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::jr(int a,int b,int c)//15
{
    if(reg[31]>=0&&reg[31]<=file.size()){
        file.seek(reg[31]);
        start=reg[31]/4;
    }
    QString s=ui->plainTextEdit->toPlainText();
    s+="jr $ra";
    if(reg[31]<0||reg[31]>file.size()){
        s+=" # 跳转地址溢出";
    }
    s+="\r\n";
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::lw(int a,int b,int c)//16
{
    QString s=ui->plainTextEdit->toPlainText();
    reg[a]=(unsigned char)store[40000+c];
    reg[a]=(unsigned int)reg[a]<<8;res+=4;
    reg[a]=(unsigned char)store[40000+c+1];
    reg[a]=(unsigned int)reg[a]<<8;
    reg[a]=(unsigned char)store[40000+c+2];
    reg[a]=(unsigned int)reg[a]<<8;
    reg[a]=(unsigned char)store[40000+c+c];
    s+=QString::number(count)+" lw ";
    s=s+lab[a+32]->text()+" , "+"("+QString::number(c)+")"+lab[b+32]->text()+"\r\n";
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::sw(int a,int b,int c)//17
{
    QString s=ui->plainTextEdit->toPlainText();
    store[reg[b]+c+3+40000]=reg[a]&255;
    reg[a]=((unsigned int)reg[a])>>8;
    store[reg[b]+c+2+40000]=reg[a]&255;
    reg[a]=((unsigned int)reg[a])>>8;
    store[reg[b]+c+1+40000]=reg[a]&255;
    reg[a]=((unsigned int)reg[a])>>8;
    store[reg[b]+c+40000]=reg[a]&255;
    s+=QString::number(count)+" sw ";
    s=s+lab[a+32]->text()+" , "+"("+QString::number(c)+")"+lab[b+32]->text()+"\r\n";
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::addi(int a,int b,int c)//18
{
    QString s=ui->plainTextEdit->toPlainText();
    reg[a]=reg[b]+c;
    s+=QString::number(count)+" addi ";
    s=s+lab[a+32]->text()+" , "+lab[b+32]->text()+" , "+QString::number(c)+"\r\n";
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::sdc2(int a,int b,int c)//19
{
    QString s=ui->plainTextEdit->toPlainText();
    store[reg[b]+c+3+40000]=reg[a]&255;
    reg[a]=((unsigned int)reg[a])>>8;
    store[reg[b]+c+2+40000]=reg[a]&255;
    reg[a]=((unsigned int)reg[a])>>8;
    store[reg[b]+c+1+40000]=reg[a]&255;
    reg[a]=((unsigned int)reg[a])>>8;
    store[reg[b]+c+40000]=reg[a]&255;
    store[reg[b]+c+3+40000]=reg[a+1]&255;
    reg[a+1]=((unsigned int)reg[a+1])>>8;
    store[reg[b]+c+2+40000]=reg[a+1]&255;
    reg[a+1]=((unsigned int)reg[a+1])>>8;
    store[reg[b]+c+1+40000]=reg[a+1]&255;
    reg[a+1]=((unsigned int)reg[a+1])>>8;
    store[reg[b]+c+40000]=reg[a+1]&255;
    s+=QString::number(count)+" sdc2 ";
    s=s+lab[a+32]->text()+" , "+"("+QString::number(c)+")"+lab[b+32]->text()+"\r\n";
    ui->plainTextEdit->setPlainText(s);
}


void MainWindow::lb(int a,int b,int c)//20
{
    QString s=ui->plainTextEdit->toPlainText();
    int temp=reg[a]&255;res++;
    reg[a]=store[reg[b]+c+40000];
    s+=QString::number(count)+" lb ";
    s=s+lab[a+32]->text()+" , "+"("+QString::number(c)+")"+lab[b+32]->text()+"\r\n";
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::swc1(int a,int b,int c)//21
{
    QString s=ui->plainTextEdit->toPlainText();
    store[reg[b]+c+3+40000]=reg[a]&255;
    reg[a]=((unsigned int)reg[a])>>8;
    store[reg[b]+c+2+40000]=reg[a]&255;
    reg[a]=((unsigned int)reg[a])>>8;
    store[reg[b]+c+1+40000]=reg[a]&255;
    reg[a]=((unsigned int)reg[a])>>8;
    store[reg[b]+c+40000]=reg[a]&255;
    s+=QString::number(count)+" swc1 ";
    s=s+lab[a+32]->text()+" , "+"("+QString::number(c)+")"+lab[b+32]->text()+"\r\n";
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::swc2(int a,int b,int c)//22
{
    QString s=ui->plainTextEdit->toPlainText();
    store[reg[b]+c+3+40000]=reg[a]&255;
    reg[a]=((unsigned int)reg[a])>>8;
    store[reg[b]+c+2+40000]=reg[a]&255;
    reg[a]=((unsigned int)reg[a])>>8;
    store[reg[b]+c+1+40000]=reg[a]&255;
    reg[a]=((unsigned int)reg[a])>>8;
    store[reg[b]+c+40000]=reg[a]&255;
    s+=QString::number(count)+" swc2 ";
    s=s+lab[a+32]->text()+" , "+"("+QString::number(c)+")"+lab[b+32]->text()+"\r\n";
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::slti(int a,int b,int c)//23
{
    if(reg[b]<c)
        reg[a]=1;
    else reg[a]=0;
    QString s=ui->plainTextEdit->toPlainText();
    s+=QString::number(count)+" slti ";
    s=s+lab[a+32]->text()+" , "+lab[b+32]->text()+" , "+QString::number(c)+"\r\n";
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::mthi(int a,int b,int c)//24
{
    hi=reg[a];
    QString s=ui->plainTextEdit->toPlainText();
    s+=QString::number(count)+" mthi ";
    s=s+lab[a+32]->text()+" # HI="+QString::number(hi)+" \r\n";
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::bgtz(int a, int b, int c)
{
    if(reg[a]>0)
    {
        int temp=file.pos();
        temp=temp+c;
        file.seek(temp);
    }
    QString s=ui->plainTextEdit->toPlainText();
    s+=QString::number(count)+" bgtz ";
    s=s+lab[a+32]->text()+" , "+QString::number(c);
    if((file.pos()+c)<0||(file.pos()+c)>end)
        s+=" # 跳转地址溢出";
    s+="\r\n";
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::blez(int a, int b, int c)
{
    if(reg[a]<=0)
    {
        int temp=file.pos();
        temp=temp+c;
        file.seek(temp);
    }
    QString s=ui->plainTextEdit->toPlainText();
    s+=QString::number(count)+" bgtz ";
    s=s+lab[a+32]->text()+" , "+QString::number(c);
    if((file.pos()+c)<0||(file.pos()+c)>end)
        s+=" # 跳转地址溢出";
    s+="\r\n";
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::show_pc()
{
    pc=file.pos();
    QString s=ui->plainTextEdit->toPlainText();
    QString temp;dec_to_hex(pc,temp);temp.remove(4,1);temp.insert(4,"4");
    if(file.atEnd()){s=s+"pc=END 已用内存="+QString::number(file.size()/4+res)+"\r\n";}
    else{s=s+"pc="+temp+" 已用内存="+QString::number(file.size()/4+res)+"\r\n";}
    ui->plainTextEdit->setPlainText(s);
}
