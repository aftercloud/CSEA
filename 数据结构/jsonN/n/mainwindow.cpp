#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<queue>
#include<stack>
#include<QFrame>
#include<QFile>
#include<QFileDialog>
#include<QTextStream>
#include<QClipboard>
#include<QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),file_name(),un(),re(),
    key(),value(),visit(),str(),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);is_open=false;is_create=false;sma="";
    connect(ui->actionCreate,SIGNAL(triggered()),this,SLOT(create()));
    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(open()));
    connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(save()));
    connect(ui->actionOther,SIGNAL(triggered()),this,SLOT(other()));
    connect(ui->actionCopy,SIGNAL(triggered()),this,SLOT(copy()));
    connect(ui->actionPaste,SIGNAL(triggered()),this,SLOT(paste()));
    connect(ui->actionUndo,SIGNAL(triggered()),this,SLOT(undo()));
    connect(ui->actionRedo,SIGNAL(triggered()),this,SLOT(redo()));
    connect(ui->actionEmpty,SIGNAL(triggered()),this,SLOT(empty()));
    connect(ui->actionTree,SIGNAL(triggered()),this,SLOT(tree()));
    connect(ui->actionClose,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionDelete,SIGNAL(triggered()),this,SLOT(Delete()));
    connect(ui->actionInquire,SIGNAL(triggered()),this,SLOT(Inquire()));
    forset=new QTreeWidgetItem*[40000];
    for(int i=0;i<40000;i++)
        forset[i]=0;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete[]forset;
}

void MainWindow::create()
{
    if(!is_open){
        is_create=true;
        file_name.clear();
        ui->plainTextEdit->setEnabled(true);
        ui->plainTextEdit->setFrameShape(QFrame::StyledPanel);
        ui->treeWidget->setDisabled(false);
        ui->treeWidget->setFrameShape(QFrame::StyledPanel);
    }
}

void MainWindow::open()
{
    if(!is_open){
        is_open=true;
        file_name=QFileDialog::getOpenFileName();
        QFile file(file_name);
        if(file.open(QIODevice::ReadOnly)){
            QString s="",temp;
            QTextStream out(&file);
            temp=out.readLine();
            while(temp!="")
            {
                s+=temp;
                s+="\r\n";
                temp=out.readLine();
            }
            file.close();
            ui->plainTextEdit->setEnabled(true);
            ui->plainTextEdit->setFrameShape(QFrame::StyledPanel);
            ui->plainTextEdit->setPlainText(s);
            ui->treeWidget->setDisabled(false);
            ui->treeWidget->setFrameShape(QFrame::StyledPanel);
        }
    }
}

void MainWindow::save()
{

    if(!is_open){
        if(!is_create)return;
        file_name=QFileDialog::getSaveFileName();
    }
    QFile file(file_name);
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream out(&file);
        QString s=ui->plainTextEdit->toPlainText();
        out<<s;
        file.close();
    }
}

void MainWindow::other()
{
    if(is_open||is_create){
        QString fi_name=QFileDialog::getSaveFileName();
        QFile file(fi_name);
        if(file.open(QIODevice::WriteOnly))
        {
            QTextStream out(&file);
            QString s=ui->plainTextEdit->toPlainText();
            out<<s;
            file.close();
        }
    }
}

void MainWindow::copy()
{
    QClipboard*clipboard=QApplication::clipboard();
    QString s=ui->plainTextEdit->toPlainText();
    clipboard->setText(s);
}

void MainWindow::paste()
{
    QClipboard*clipboard=QApplication::clipboard();
    QString s=ui->plainTextEdit->toPlainText();
    s+=clipboard->text();
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::undo()
{
    if(un.size()==0)return;
    QString s=ui->plainTextEdit->toPlainText();
    if(re.size()>=10)
        re.erase(re.begin());
    re.push_back(s);
    if(s==un[un.size()-1])
        un.erase(un.end());
    s=un[un.size()-1];
    un.erase(un.end());
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::redo()
{
    if(re.size()==0)return;
    QString s=re[re.size()-1];
    re.erase(re.end());
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::empty()
{
    QString s=ui->plainTextEdit->toPlainText();
    int num=s.length();
    for(int i=0;i<num;i++){
        if(s[i].toLatin1()==10||s[i].toLatin1()==9||s[i].toLatin1()==13||s[i].toLatin1()==32)
            s.remove(i--,1);
    }
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::tree()
{
    Delete();//清空树
    key.clear();value.clear();
    visit.clear();str.clear();
    QString s=ui->plainTextEdit->toPlainText();
    int left=0,right=0,medium=0;//左右引号
    int num=s.length();
    bool jump=true;
    for(int i=0;i<num;i++){
        if(s[i].toLatin1()==34)jump=!jump;
        if((s[i].toLatin1()==32||s[i].toLatin1()==9||s[i].toLatin1()==10)&&(jump))
            s.remove(i--,1);
    }
    QString s_temp=s;QString temp="";
    while(right>=0){//提取数据
        left=s.indexOf("\"");
        s.remove(0,left+1);
        right=s.indexOf("\"");
        temp=s.left(right);
        s.remove(0,right+1);
        key.push_back(temp);//将键放入向量
        medium=s.indexOf(":");
        if(s[0].toLatin1()==34||s[medium+1].toLatin1()==91||s[medium+1].toLatin1()==123)//:后面是[或{
        {
            visit.push_back(false);
            str.push_back(false);
            value.push_back(" ");
        }
        else if(s[medium+1].toLatin1()==34)//:后面是"
        {
            left=s.indexOf("\"");
            s.remove(0,left+1);
            right=s.indexOf("\"");
            temp=s.left(right);
            s.remove(0,right+1);
            value.push_back(temp);//将值放入向量
            visit.push_back(true);
            str.push_back(true);
        }
        else//value是数值
        {
            int length=medium+2;
            while(s[length].toLatin1()==46||(s[length].toLatin1()>=48&&s[length].toLatin1()<=57)||
                  (s[length].toLatin1()>=65&&s[length].toLatin1()<=90)||(s[length].toLatin1()>=97&&s[length].toLatin1()<=122))
                length++;
            temp=s.mid(medium+1,length-1-medium);
            value.push_back(temp);
            visit.push_back(true);
            str.push_back(false);
        }
        right=s.indexOf("\"");
    }
    right++;
    s="";
    for(int i=0;i<visit.size();i++){
        s+=key[i];
        if(visit[i])
            s+=value[i];
        s+="\r\n";
    }
    s=s_temp;int v=0;QString vs="";
    for(int i=0;i<visit.size();i++){
        vs=key[i];
        v=s.indexOf(vs);
        s.remove(v,vs.size());
        s.insert(v,"!");
        vs=value[i];
        if(vs.size()!=0&&vs!=" "){
            v=s.indexOf(vs);
            s.remove(v,vs.size());
        }
    }
    for(int i=0;i<s.size();i++){
        if(s[i].toLatin1()==34||s[i].toLatin1()==58){
            s.remove(i--,1);
        }
    }
    sma=s;
    int Num=0;int Count=0;//Num表示树形，Count表示数据
    QTreeWidgetItem*pt=0;
    std::stack<QTreeWidgetItem*>tree;
    int judge=0;//在方括号内的层数
    int numb=0;//当前层方括号的值
    std::stack<int>jn;//每一层方括号的值
    forset[Num++]=new QTreeWidgetItem(ui->treeWidget,QStringList("JSON"));
    for(int i=0;i<s.size();i++){
        if(s[i].toLatin1()==123){
            if(judge==0)
                tree.push(forset[Num-1]);
            else{
                if(s[i-2].toLatin1()==44)numb=0;
                pt=tree.top();QString all="[ ] ";all+=QString::number(numb++);
                forset[Num++]=new QTreeWidgetItem(pt,QStringList(all));
                tree.push(forset[Num-1]);
            }
        }
        else if(s[i].toLatin1()==33){
            pt=tree.top();
            QString all=key[Count++];
            if(visit[Count-1]){
                if(str[Count-1]){
                    all=all+":\""+value[Count-1]+"\"";
                }
                else{
                    all=all+":"+value[Count-1];
                }
            }
            forset[Num++]=new QTreeWidgetItem(pt,QStringList(all));
        }
        else if(s[i].toLatin1()==91){
            if(judge++>0){
                jn.push(numb);
                numb=0;
            }
            if(s[i-1].toLatin1()!=44){
                tree.push(forset[Num-1]);
                jn.push(numb);
                numb=0;
            }
            else{
                pt=tree.top();
                if(jn.size()>0){
                    numb=jn.top();
                    jn.pop();
                }
                QString all="[ ] ";all+=QString::number(numb++);
                forset[Num++]=new QTreeWidgetItem(pt,QStringList(all));
                tree.push(forset[Num-1]);
            }
        }
        else if(s[i].toLatin1()==44){
            if(s[i+1].toLatin1()==33){
                pt=tree.top();i++;
                QString all=key[Count++];
                if(visit[Count-1]){
                    if(str[Count-1]){
                        all=all+":\""+value[Count-1]+"\"";
                    }
                    else{
                        all=all+":"+value[Count-1];
                    }
                }
                forset[Num++]=new QTreeWidgetItem(pt,QStringList(all));
            }
        }
        else if(s[i].toLatin1()==125){
            tree.pop();
        }
        else if(s[i].toLatin1()==93){
            tree.pop();
            --judge;
            if(jn.size()>0){
                numb=jn.top();
                jn.pop();
            }
        }
    }
}

void MainWindow::close()
{
    this->Delete();
    un.clear();
    re.clear();
    is_open=false;is_create=false;
    file_name.clear();
    ui->plainTextEdit->clear();
    ui->plainTextEdit->setDisabled(true);
    ui->plainTextEdit->setFrameShape(QFrame::NoFrame);
    ui->treeWidget->setDisabled(true);
    ui->treeWidget->setFrameShape(QFrame::NoFrame);
    ui->lineEdit->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->plainTextEdit_2->clear();
    ui->plainTextEdit_2->setEnabled(false);
    ui->plainTextEdit_2->setFrameShape(QFrame::NoFrame);
}

void MainWindow::on_plainTextEdit_textChanged()
{
    QString s=ui->plainTextEdit->toPlainText();
    int aaa=un.size();
    if(un.size()>=10)
        un.erase(un.begin());
    un.push_back(s);
    aaa=un.size();
}

void MainWindow::Delete()
{
    for(int i=39999;i>-1;i--)
    {
        if(forset[i]!=0)
            delete forset[i];
    }
}

void MainWindow::Inquire()
{
    ui->lineEdit->setEnabled(true);
    ui->pushButton->setEnabled(true);
    ui->plainTextEdit_2->setEnabled(true);
    ui->plainTextEdit_2->setFrameShape(QFrame::StyledPanel);
}

void MainWindow::on_pushButton_clicked()
{
    QString inq=ui->lineEdit->text();
    if(inq.indexOf("$.")!=0)return;
    inq.remove(0,2);
    QString output="";
    QTreeWidgetItem*pt=forset[0];//JSON指针
    int count=pt->childCount();
    if(inq[0].toLatin1()==46){//$..
        if(inq.indexOf("[")==-1){//无区间
            QString search=inq.mid(1);
            if(search=="*"){//all
                for(int i=0;i<visit.size();i++){
                    if(visit[i]){
                        output+=key[i];
                        output+=":";
                        output+=value[i];
                        output+="\r\n";
                    }
                }
            }
            else{
                for(int i=0;i<visit.size();i++){
                    if(key[i]==search){
                        output+=value[i];
                        output+="\r\n";
                    }
                }
            }
        }
        else{//有区间
            QString search=inq.mid(1,inq.indexOf("[")-1);
            int locate=0;
            while(1){//找到父节点
                if(search==forset[locate++]->text(0))
                    break;
            }
            --locate;
            inq.remove(0,inq.indexOf("[")+1);
            inq.remove(inq.size()-1,1);
            pt=forset[locate];
            if(inq.indexOf("?")!=-1){//条件查询
                if(inq.indexOf("|")==-1&&inq.indexOf("&")==-1)//单条件查询
                {
                    if(inq.indexOf("<")==-1&&inq.indexOf("=")==-1&&inq.indexOf(">")==-1){//存在查询
                        inq.remove(inq.size()-1,1);
                        inq.remove(0,inq.indexOf(".")+1);
                        for(int j=0;j<pt->childCount();j++){
                            for(int k=0;k<pt->child(j)->childCount();k++){
                                if(pt->child(j)->child(k)->text(0).indexOf(inq)!=-1){
                                    for(int m=0;m<pt->child(j)->childCount();m++){
                                        output+=pt->child(j)->child(m)->text(0);
                                        output+="\r\n";
                                    }
                                }
                            }
                        }
                    }
                    else{//比较查询
                        inq.remove(0,inq.indexOf(".")+1);
                        inq.remove(inq.indexOf(")"),inq.size()-inq.indexOf(")"));
                        if(inq.indexOf(">")!=-1){
                            int loc=inq.indexOf(">");
                            if(inq[loc+1].toLatin1()==61) {//>=
                                double compare=inq.mid(inq.indexOf("=")+1).toDouble();
                                inq.remove(inq.indexOf(">"),inq.size()-inq.indexOf(">"));
                                for(int j=0;j<pt->childCount();j++){
                                    for(int k=0;k<pt->child(j)->childCount();k++){
                                        QString tem=pt->child(j)->child(k)->text(0);
                                        if(tem.indexOf(inq)!=-1&&tem.mid(tem.indexOf(":")+1).toDouble()>=compare){
                                            for(int m=0;m<pt->child(j)->childCount();m++){
                                                output+=pt->child(j)->child(m)->text(0);
                                                output+="\r\n";
                                            }
                                        }
                                    }
                                }
                            }
                            else{//>
                                double compare=inq.mid(inq.indexOf(">")+1).toDouble();
                                inq.remove(inq.indexOf(">"),inq.size()-inq.indexOf(">"));
                                for(int j=0;j<pt->childCount();j++){
                                    for(int k=0;k<pt->child(j)->childCount();k++){
                                        QString tem=pt->child(j)->child(k)->text(0);
                                        if(tem.indexOf(inq)!=-1&&tem.mid(tem.indexOf(":")+1).toDouble()>compare){
                                            for(int m=0;m<pt->child(j)->childCount();m++){
                                                output+=pt->child(j)->child(m)->text(0);
                                                output+="\r\n";
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else if(inq.indexOf("<")!=-1){
                            int loc=inq.indexOf("<");
                            if(inq[loc+1].toLatin1()==61){//<=
                                double compare=inq.mid(inq.indexOf("=")+1).toDouble();
                                inq.remove(inq.indexOf("<"),inq.size()-inq.indexOf("<"));
                                for(int j=0;j<pt->childCount();j++){
                                    for(int k=0;k<pt->child(j)->childCount();k++){
                                        QString tem=pt->child(j)->child(k)->text(0);
                                        if(tem.indexOf(inq)!=-1&&tem.mid(tem.indexOf(":")+1).toDouble()<=compare){
                                            for(int m=0;m<pt->child(j)->childCount();m++){
                                                output+=pt->child(j)->child(m)->text(0);
                                                output+="\r\n";
                                            }
                                        }
                                    }
                                }
                            }
                            else{//<
                                double compare=inq.mid(inq.indexOf("<")+1).toDouble();
                                inq.remove(inq.indexOf("<"),inq.size()-inq.indexOf("<"));
                                for(int j=0;j<pt->childCount();j++){
                                    for(int k=0;k<pt->child(j)->childCount();k++){
                                        QString tem=pt->child(j)->child(k)->text(0);
                                        if(tem.indexOf(inq)!=-1&&tem.mid(tem.indexOf(":")+1).toDouble()<compare){
                                            for(int m=0;m<pt->child(j)->childCount();m++){
                                                output+=pt->child(j)->child(m)->text(0);
                                                output+="\r\n";
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else{//==
                            int loc=inq.indexOf("=");
                            QString compare=inq.mid(inq.indexOf("=")+2);
                            if(compare[0].toLatin1()==34){compare.remove(0,1);compare.remove(compare.size()-1,1);}
                            inq.remove(inq.indexOf("="),inq.size()-inq.indexOf("="));
                            for(int j=0;j<pt->childCount();j++){
                                for(int k=0;k<pt->child(j)->childCount();k++){
                                    QString tem=pt->child(j)->child(k)->text(0);
                                    QString te=tem.mid(tem.indexOf(":")+1);
                                    if(te[0].toLatin1()==34){te.remove(0,1);te.remove(te.size()-1,1);}
                                    if(tem.indexOf(inq)!=-1&&te==compare){
                                        for(int m=0;m<pt->child(j)->childCount();m++){
                                            output+=pt->child(j)->child(m)->text(0);
                                            output+="\r\n";
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                else{//复合条件查询
                    bool boolean=false;
                    QString sto=inq;
                    if(sto.indexOf("|")==-1){//与
                        boolean=true;
                        inq.remove(0,inq.indexOf(".")+1);
                        inq.remove(inq.indexOf("&"),inq.size()-inq.indexOf("&"));
                        sto.remove(0,sto.indexOf("&")+2);
                        sto.remove(0,sto.indexOf(".")+1);
                        sto.remove(sto.size()-1,1);
                    }
                    else{//或
                        boolean=false;
                        inq.remove(0,inq.indexOf(".")+1);
                        inq.remove(inq.indexOf("|"),inq.size()-inq.indexOf("|"));
                        sto.remove(0,sto.indexOf("|")+2);
                        sto.remove(0,sto.indexOf(".")+1);
                        sto.remove(sto.size()-1,1);
                    }
                    std::vector<int>r1;
                    std::vector<int>r2;
                    if(inq.indexOf(">")!=-1){
                        int loc=inq.indexOf(">");
                        if(inq[loc+1].toLatin1()==61) {//>=
                            double compare=inq.mid(inq.indexOf("=")+1).toDouble();
                            inq.remove(inq.indexOf(">"),inq.size()-inq.indexOf(">"));
                            for(int j=0;j<pt->childCount();j++){
                                for(int k=0;k<pt->child(j)->childCount();k++){
                                    QString tem=pt->child(j)->child(k)->text(0);
                                    if(tem.indexOf(inq)!=-1&&tem.mid(tem.indexOf(":")+1).toDouble()>=compare){
                                        r1.push_back(j);
                                    }
                                }
                            }
                        }
                        else{//>
                            double compare=inq.mid(inq.indexOf(">")+1).toDouble();
                            inq.remove(inq.indexOf(">"),inq.size()-inq.indexOf(">"));
                            for(int j=0;j<pt->childCount();j++){
                                for(int k=0;k<pt->child(j)->childCount();k++){
                                    QString tem=pt->child(j)->child(k)->text(0);
                                    if(tem.indexOf(inq)!=-1&&tem.mid(tem.indexOf(":")+1).toDouble()>compare){
                                        r1.push_back(j);
                                    }
                                }
                            }
                        }
                    }
                    else if(inq.indexOf("<")!=-1){
                        int loc=inq.indexOf("<");
                        if(inq[loc+1].toLatin1()==61){//<=
                            double compare=inq.mid(inq.indexOf("=")+1).toDouble();
                            inq.remove(inq.indexOf("<"),inq.size()-inq.indexOf("<"));
                            for(int j=0;j<pt->childCount();j++){
                                for(int k=0;k<pt->child(j)->childCount();k++){
                                    QString tem=pt->child(j)->child(k)->text(0);
                                    if(tem.indexOf(inq)!=-1&&tem.mid(tem.indexOf(":")+1).toDouble()<=compare){
                                        r1.push_back(j);
                                    }
                                }
                            }
                        }
                        else{//<
                            double compare=inq.mid(inq.indexOf("<")+1).toDouble();
                            inq.remove(inq.indexOf("<"),inq.size()-inq.indexOf("<"));
                            for(int j=0;j<pt->childCount();j++){
                                for(int k=0;k<pt->child(j)->childCount();k++){
                                    QString tem=pt->child(j)->child(k)->text(0);
                                    if(tem.indexOf(inq)!=-1&&tem.mid(tem.indexOf(":")+1).toDouble()<compare){
                                        r1.push_back(j);
                                    }
                                }
                            }
                        }
                    }
                    else{//==
                        int loc=inq.indexOf("=");
                        QString compare=inq.mid(inq.indexOf("=")+2);
                        if(compare[0].toLatin1()==34){compare.remove(0,1);compare.remove(compare.size()-1,1);}
                        inq.remove(inq.indexOf("="),inq.size()-inq.indexOf("="));
                        for(int j=0;j<pt->childCount();j++){
                            for(int k=0;k<pt->child(j)->childCount();k++){
                                QString tem=pt->child(j)->child(k)->text(0);
                                QString te=tem.mid(tem.indexOf(":")+1);
                                if(te[0].toLatin1()==34){te.remove(0,1);te.remove(te.size()-1,1);}
                                if(tem.indexOf(inq)!=-1&&te==compare){
                                    r1.push_back(j);
                                }
                            }
                        }
                    }
                    inq=sto;
                    if(inq.indexOf(">")!=-1){
                        int loc=inq.indexOf(">");
                        if(inq[loc+1].toLatin1()==61) {//>=
                            double compare=inq.mid(inq.indexOf("=")+1).toDouble();
                            inq.remove(inq.indexOf(">"),inq.size()-inq.indexOf(">"));
                            for(int j=0;j<pt->childCount();j++){
                                for(int k=0;k<pt->child(j)->childCount();k++){
                                    QString tem=pt->child(j)->child(k)->text(0);
                                    if(tem.indexOf(inq)!=-1&&tem.mid(tem.indexOf(":")+1).toDouble()>=compare){
                                        r2.push_back(j);
                                    }
                                }
                            }
                        }
                        else{//>
                            double compare=inq.mid(inq.indexOf(">")+1).toDouble();
                            inq.remove(inq.indexOf(">"),inq.size()-inq.indexOf(">"));
                            for(int j=0;j<pt->childCount();j++){
                                for(int k=0;k<pt->child(j)->childCount();k++){
                                    QString tem=pt->child(j)->child(k)->text(0);
                                    if(tem.indexOf(inq)!=-1&&tem.mid(tem.indexOf(":")+1).toDouble()>compare){
                                        r2.push_back(j);
                                    }
                                }
                            }
                        }
                    }
                    else if(inq.indexOf("<")!=-1){
                        int loc=inq.indexOf("<");
                        if(inq[loc+1].toLatin1()==61){//<=
                            double compare=inq.mid(inq.indexOf("=")+1).toDouble();
                            inq.remove(inq.indexOf("<"),inq.size()-inq.indexOf("<"));
                            for(int j=0;j<pt->childCount();j++){
                                for(int k=0;k<pt->child(j)->childCount();k++){
                                    QString tem=pt->child(j)->child(k)->text(0);
                                    if(tem.indexOf(inq)!=-1&&tem.mid(tem.indexOf(":")+1).toDouble()<=compare){
                                        r2.push_back(j);
                                    }
                                }
                            }
                        }
                        else{//<
                            double compare=inq.mid(inq.indexOf("<")+1).toDouble();
                            inq.remove(inq.indexOf("<"),inq.size()-inq.indexOf("<"));
                            for(int j=0;j<pt->childCount();j++){
                                for(int k=0;k<pt->child(j)->childCount();k++){
                                    QString tem=pt->child(j)->child(k)->text(0);
                                    if(tem.indexOf(inq)!=-1&&tem.mid(tem.indexOf(":")+1).toDouble()<compare){
                                        r2.push_back(j);
                                    }
                                }
                            }
                        }
                    }
                    else{//==
                        int loc=inq.indexOf("=");
                        QString compare=inq.mid(inq.indexOf("=")+2);
                        if(compare[0].toLatin1()==34){compare.remove(0,1);compare.remove(compare.size()-1,1);}
                        inq.remove(inq.indexOf("="),inq.size()-inq.indexOf("="));
                        for(int j=0;j<pt->childCount();j++){
                            for(int k=0;k<pt->child(j)->childCount();k++){
                                QString tem=pt->child(j)->child(k)->text(0);
                                QString te=tem.mid(tem.indexOf(":")+1);
                                if(te[0].toLatin1()==34){te.remove(0,1);te.remove(te.size()-1,1);}
                                if(tem.indexOf(inq)!=-1&&te==compare){
                                    r2.push_back(j);
                                }
                            }
                        }
                    }
                    std::vector<int>res;
                    if(boolean){//与
                        for(int t=0;t<r1.size();t++){
                            for(int T=0;T<r2.size();T++){
                                if(r1[t]==r2[T])
                                    res.push_back(r1[t]);
                            }
                        }
                    }
                    else{//或
                        for(int t=0;t<r1.size();t++){
                            bool similar=false;
                            for(int T=0;T<r2.size();T++){
                                if(r1[t]==r2[T])
                                    similar=true;
                            }
                            if(!similar)
                                res.push_back(r1[t]);
                        }
                        for(int t=0;t<r2.size();t++)
                            res.push_back(r2[t]);
                    }
                    for(int t=0;t<res.size();t++){
                        for(int T=0;T<pt->child(res[t])->childCount();T++){
                            output+=pt->child(res[t])->child(T)->text(0);
                            output+="\r\n";
                        }
                    }
                }
            }
            else if(inq.indexOf(":")==-1&&inq.indexOf("@")==-1){
                while(inq.indexOf(",")!=-1){
                    int target=inq.mid(0,inq.indexOf(",")).toInt();
                    inq.remove(0,inq.indexOf(",")+1);
                    pt=forset[locate]->child(target);
                    for(int j=0;j<pt->childCount();j++){
                        output+=pt->child(j)->text(0);
                        output+="\r\n";
                    }
                }
                int target=inq.toInt();
                pt=forset[locate]->child(target);
                for(int j=0;j<pt->childCount();j++){
                    output+=pt->child(j)->text(0);
                    output+="\r\n";
                }
            }
            else if(inq.indexOf(":")!=-1)
            {
                inq.remove(inq.indexOf(":"),1);
                int target=inq.toInt();
                if(target>0){
                    for(int k=0;k<target;k++){
                        pt=forset[locate]->child(k);
                        for(int j=0;j<pt->childCount();j++){
                            output+=pt->child(j)->text(0);
                            output+="\r\n";
                        }
                    }
                }
                else{
                    target*=-1;
                    for(int k=0;k<target;k++){
                        pt=forset[locate]->child(pt->childCount()-k-1);
                        for(int j=0;j<pt->childCount();j++){
                            output+=pt->child(j)->text(0);
                            output+="\r\n";
                        }
                    }
                }
            }
            else{//@
                inq.remove(0,inq.indexOf("-")+1);
                inq.remove(inq.size()-1,1);
                int target=inq.toInt();
                for(int k=0;k<target;k++){
                    pt=forset[locate]->child(pt->childCount()-k-1);
                    for(int j=0;j<pt->childCount();j++){
                        output+=pt->child(j)->text(0);
                        output+="\r\n";
                    }
                }
            }
        }
    }
    else{
        if(inq.indexOf("[")==-1){
            QString sa=sma;
            QString search=inq.mid(0,inq.indexOf("."));
            int locate=0;
            while(1){//找到父节点
                if(search==forset[locate++]->text(0))
                    break;
            }
            --locate;
            inq.remove(0,inq.indexOf(".")+1);
            pt=forset[locate];
            if(inq[0].toLatin1()==42){
                int sta=1;int loc=1;
                while(1){
                    if(loc==locate){
                        sa.remove(0,sa.indexOf("!")+1);
                        break;
                    }
                    loc++;
                    sa.remove(0,sa.indexOf("!")+2);
                }
                sa.remove(0,1);
                while(sta>0){
                    if(sa[0].toLatin1()==91||sa[0].toLatin1()==123)sta++;
                    else if(sa[0].toLatin1()==33){
                        locate++;
                        if(forset[locate]->text(0)[0].toLatin1()==91)locate++;
                        output+=forset[locate]->text(0);
                        output+="\r\n";
                    }
                    else if(sa[0].toLatin1()==93||sa[0].toLatin1()==125)sta--;
                    sa.remove(0,1);
                }
            }
            else if(inq[0].toLatin1()==46){
                inq.remove(0,1);
                int sta=1;int loc=1;
                while(1){
                    if(loc==locate){
                        sa.remove(0,sa.indexOf("!")+1);
                        break;
                    }
                    loc++;
                    sa.remove(0,sa.indexOf("!")+2);
                }
                sa.remove(0,1);
                while(sta>0){
                    if(sa[0].toLatin1()==91||sa[0].toLatin1()==123)sta++;
                    else if(sa[0].toLatin1()==33){
                        locate++;
                        if(forset[locate]->text(0)[0].toLatin1()==91)locate++;
                        if(forset[locate]->text(0).indexOf(inq)!=-1){
                            output+=forset[locate]->text(0);
                            output+="\r\n";
                        }
                    }
                    else if(sa[0].toLatin1()==93||sa[0].toLatin1()==125)sta--;
                    sa.remove(0,1);
                }
            }
        }
        else{
            if(inq.indexOf("?")==-1){
                QString search=inq.mid(0,inq.indexOf("."));
                int locate=0;
                while(1){//找到父节点
                    if(search==forset[locate++]->text(0))
                        break;
                }
                --locate;
                inq.remove(0,inq.indexOf(".")+1);
                pt=forset[locate];
                QTreeWidgetItem*ptr=0;
                search=inq.mid(0,inq.indexOf("["));
                for(int i=0;i<pt->childCount();i++){
                    if(search==pt->child(i)->text(0)){
                        pt=pt->child(i);
                        break;
                    }
                }
                inq.remove(0,inq.indexOf("[")+1);
                if(inq.size()-1!=inq.indexOf("]")){
                    search=inq.mid(inq.indexOf("]")+2);
                }
                else {
                    search="";
                    inq.remove(inq.size()-1,1);
                }
                if(inq.indexOf("*")==-1){
                    while(inq.indexOf(",")!=-1){
                        int target=inq.mid(0,inq.indexOf(",")).toInt();
                        inq.remove(0,inq.indexOf(",")+1);
                        ptr=pt->child(target);
                        for(int j=0;j<ptr->childCount();j++){
                            if(search.size()!=0){
                                if(ptr->child(j)->text(0).indexOf(search)!=-1){
                                    output+=ptr->child(j)->text(0);
                                    output+="\r\n";
                                }
                            }
                            else{
                                output+=ptr->child(j)->text(0);
                                output+="\r\n";
                            }
                        }
                    }
                    int where=inq.indexOf("]");
                    inq.remove(where,inq.size()-where);
                    int target=inq.toInt();
                    ptr=pt->child(target);
                    for(int j=0;j<ptr->childCount();j++){
                        if(ptr->child(j)->text(0).indexOf(search)!=-1){
                            output+=ptr->child(j)->text(0);
                            output+="\r\n";
                        }
                    }
                }
                else{
                    for(int l=0;l<pt->childCount();l++){
                        ptr=pt->child(l);
                        for(int j=0;j<ptr->childCount();j++){
                            if(ptr->child(j)->text(0).indexOf(search)!=-1){
                                output+=ptr->child(j)->text(0);
                                output+="\r\n";
                            }
                        }
                    }
                }
            }
            else{//从父节点找到父节点
                QString search=inq.mid(0,inq.indexOf("."));
                inq.remove(0,inq.indexOf(".")+1);
                int locate=0;
                while(1){//找到父节点
                    if(search==forset[locate++]->text(0))
                        break;
                }
                --locate;
                pt=forset[locate];
                search=inq.mid(0,inq.indexOf("["));
                inq.remove(0,inq.indexOf("[")+1);
                locate=0;
                while(1){//找到父节点
                    if(search==pt->child(locate)->text(0))
                        break;
                    locate++;
                }
                pt=pt->child(locate);QString tail;
                if(inq.indexOf("]")==inq.size()-1){
                    tail="";
                    inq.remove(inq.size()-1,1);
                }
                else{
                    tail=inq.mid(inq.indexOf("]")+2);
                    inq.remove(inq.indexOf("]"),inq.size()-inq.indexOf("]"));
                }
                if(inq.indexOf("|")==-1&&inq.indexOf("&")==-1){
                    inq.remove(0,inq.indexOf(".")+1);
                    inq.remove(inq.indexOf(")"),inq.size()-inq.indexOf(")"));
                    if(inq.indexOf(">")!=-1){
                        int loc=inq.indexOf(">");
                        if(inq[loc+1].toLatin1()==61) {//>=
                            double compare=inq.mid(inq.indexOf("=")+1).toDouble();
                            inq.remove(inq.indexOf(">"),inq.size()-inq.indexOf(">"));
                            for(int j=0;j<pt->childCount();j++){
                                for(int k=0;k<pt->child(j)->childCount();k++){
                                    QString tem=pt->child(j)->child(k)->text(0);
                                    if(tem.indexOf(inq)!=-1&&tem.mid(tem.indexOf(":")+1).toDouble()>=compare){
                                        for(int m=0;m<pt->child(j)->childCount();m++){
                                            if(tail.size()==0){
                                                output+=pt->child(j)->child(m)->text(0);
                                                output+="\r\n";
                                            }
                                            else if(pt->child(j)->child(m)->text(0).indexOf(tail)!=-1){
                                                output+=pt->child(j)->child(m)->text(0);
                                                output+="\r\n";
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else{//>
                            double compare=inq.mid(inq.indexOf(">")+1).toDouble();
                            inq.remove(inq.indexOf(">"),inq.size()-inq.indexOf(">"));
                            for(int j=0;j<pt->childCount();j++){
                                for(int k=0;k<pt->child(j)->childCount();k++){
                                    QString tem=pt->child(j)->child(k)->text(0);
                                    if(tem.indexOf(inq)!=-1&&tem.mid(tem.indexOf(":")+1).toDouble()>compare){
                                        for(int m=0;m<pt->child(j)->childCount();m++){
                                            if(tail.size()==0){
                                                output+=pt->child(j)->child(m)->text(0);
                                                output+="\r\n";
                                            }
                                            else if(pt->child(j)->child(m)->text(0).indexOf(tail)!=-1){
                                                output+=pt->child(j)->child(m)->text(0);
                                                output+="\r\n";
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else if(inq.indexOf("<")!=-1){
                        int loc=inq.indexOf("<");
                        if(inq[loc+1].toLatin1()==61){//<=
                            double compare=inq.mid(inq.indexOf("=")+1).toDouble();
                            inq.remove(inq.indexOf("<"),inq.size()-inq.indexOf("<"));
                            for(int j=0;j<pt->childCount();j++){
                                for(int k=0;k<pt->child(j)->childCount();k++){
                                    QString tem=pt->child(j)->child(k)->text(0);
                                    if(tem.indexOf(inq)!=-1&&tem.mid(tem.indexOf(":")+1).toDouble()<=compare){
                                        for(int m=0;m<pt->child(j)->childCount();m++){
                                            if(tail.size()==0){
                                                output+=pt->child(j)->child(m)->text(0);
                                                output+="\r\n";
                                            }
                                            else if(pt->child(j)->child(m)->text(0).indexOf(tail)!=-1){
                                                output+=pt->child(j)->child(m)->text(0);
                                                output+="\r\n";
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else{//<
                            double compare=inq.mid(inq.indexOf("<")+1).toDouble();
                            inq.remove(inq.indexOf("<"),inq.size()-inq.indexOf("<"));
                            for(int j=0;j<pt->childCount();j++){
                                for(int k=0;k<pt->child(j)->childCount();k++){
                                    QString tem=pt->child(j)->child(k)->text(0);
                                    if(tem.indexOf(inq)!=-1&&tem.mid(tem.indexOf(":")+1).toDouble()<compare){
                                        for(int m=0;m<pt->child(j)->childCount();m++){
                                            if(tail.size()==0){
                                                output+=pt->child(j)->child(m)->text(0);
                                                output+="\r\n";
                                            }
                                            else if(pt->child(j)->child(m)->text(0).indexOf(tail)!=-1){
                                                output+=pt->child(j)->child(m)->text(0);
                                                output+="\r\n";
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else{//==
                        int loc=inq.indexOf("=");
                        QString compare=inq.mid(inq.indexOf("=")+2);
                        if(compare[0].toLatin1()==34){compare.remove(0,1);compare.remove(compare.size()-1,1);}
                        inq.remove(inq.indexOf("="),inq.size()-inq.indexOf("="));
                        for(int j=0;j<pt->childCount();j++){
                            for(int k=0;k<pt->child(j)->childCount();k++){
                                QString tem=pt->child(j)->child(k)->text(0);
                                QString te=tem.mid(tem.indexOf(":")+1);
                                if(te[0].toLatin1()==34){te.remove(0,1);te.remove(te.size()-1,1);}
                                if(tem.indexOf(inq)!=-1&&te==compare){
                                    for(int m=0;m<pt->child(j)->childCount();m++){
                                        if(tail.size()==0){
                                            output+=pt->child(j)->child(m)->text(0);
                                            output+="\r\n";
                                        }
                                        else if(pt->child(j)->child(m)->text(0).indexOf(tail)!=-1){
                                            output+=pt->child(j)->child(m)->text(0);
                                            output+="\r\n";
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                else{
                    bool boolean=false;
                    QString sto=inq;
                    if(sto.indexOf("|")==-1){//与
                        boolean=true;
                        inq.remove(0,inq.indexOf(".")+1);
                        inq.remove(inq.indexOf("&"),inq.size()-inq.indexOf("&"));
                        sto.remove(0,sto.indexOf("&")+2);
                        sto.remove(0,sto.indexOf(".")+1);
                        sto.remove(sto.size()-1,1);
                    }
                    else{//或
                        boolean=false;
                        inq.remove(0,inq.indexOf(".")+1);
                        inq.remove(inq.indexOf("|"),inq.size()-inq.indexOf("|"));
                        sto.remove(0,sto.indexOf("|")+2);
                        sto.remove(0,sto.indexOf(".")+1);
                        sto.remove(sto.size()-1,1);
                    }
                    std::vector<int>r1;
                    std::vector<int>r2;
                    if(inq.indexOf(">")!=-1){
                        int loc=inq.indexOf(">");
                        if(inq[loc+1].toLatin1()==61) {//>=
                            double compare=inq.mid(inq.indexOf("=")+1).toDouble();
                            inq.remove(inq.indexOf(">"),inq.size()-inq.indexOf(">"));
                            for(int j=0;j<pt->childCount();j++){
                                for(int k=0;k<pt->child(j)->childCount();k++){
                                    QString tem=pt->child(j)->child(k)->text(0);
                                    if(tem.indexOf(inq)!=-1&&tem.mid(tem.indexOf(":")+1).toDouble()>=compare){
                                        r1.push_back(j);
                                    }
                                }
                            }
                        }
                        else{//>
                            double compare=inq.mid(inq.indexOf(">")+1).toDouble();
                            inq.remove(inq.indexOf(">"),inq.size()-inq.indexOf(">"));
                            for(int j=0;j<pt->childCount();j++){
                                for(int k=0;k<pt->child(j)->childCount();k++){
                                    QString tem=pt->child(j)->child(k)->text(0);
                                    if(tem.indexOf(inq)!=-1&&tem.mid(tem.indexOf(":")+1).toDouble()>compare){
                                        r1.push_back(j);
                                    }
                                }
                            }
                        }
                    }
                    else if(inq.indexOf("<")!=-1){
                        int loc=inq.indexOf("<");
                        if(inq[loc+1].toLatin1()==61){//<=
                            double compare=inq.mid(inq.indexOf("=")+1).toDouble();
                            inq.remove(inq.indexOf("<"),inq.size()-inq.indexOf("<"));
                            for(int j=0;j<pt->childCount();j++){
                                for(int k=0;k<pt->child(j)->childCount();k++){
                                    QString tem=pt->child(j)->child(k)->text(0);
                                    if(tem.indexOf(inq)!=-1&&tem.mid(tem.indexOf(":")+1).toDouble()<=compare){
                                        r1.push_back(j);
                                    }
                                }
                            }
                        }
                        else{//<
                            double compare=inq.mid(inq.indexOf("<")+1).toDouble();
                            inq.remove(inq.indexOf("<"),inq.size()-inq.indexOf("<"));
                            for(int j=0;j<pt->childCount();j++){
                                for(int k=0;k<pt->child(j)->childCount();k++){
                                    QString tem=pt->child(j)->child(k)->text(0);
                                    if(tem.indexOf(inq)!=-1&&tem.mid(tem.indexOf(":")+1).toDouble()<compare){
                                        r1.push_back(j);
                                    }
                                }
                            }
                        }
                    }
                    else{//==
                        int loc=inq.indexOf("=");
                        QString compare=inq.mid(inq.indexOf("=")+2);
                        if(compare[0].toLatin1()==34){compare.remove(0,1);compare.remove(compare.size()-1,1);}
                        inq.remove(inq.indexOf("="),inq.size()-inq.indexOf("="));
                        for(int j=0;j<pt->childCount();j++){
                            for(int k=0;k<pt->child(j)->childCount();k++){
                                QString tem=pt->child(j)->child(k)->text(0);
                                QString te=tem.mid(tem.indexOf(":")+1);
                                if(te[0].toLatin1()==34){te.remove(0,1);te.remove(te.size()-1,1);}
                                if(tem.indexOf(inq)!=-1&&te==compare){
                                    r1.push_back(j);
                                }
                            }
                        }
                    }
                    inq=sto;
                    if(inq.indexOf(">")!=-1){
                        int loc=inq.indexOf(">");
                        if(inq[loc+1].toLatin1()==61) {//>=
                            double compare=inq.mid(inq.indexOf("=")+1).toDouble();
                            inq.remove(inq.indexOf(">"),inq.size()-inq.indexOf(">"));
                            for(int j=0;j<pt->childCount();j++){
                                for(int k=0;k<pt->child(j)->childCount();k++){
                                    QString tem=pt->child(j)->child(k)->text(0);
                                    if(tem.indexOf(inq)!=-1&&tem.mid(tem.indexOf(":")+1).toDouble()>=compare){
                                        r2.push_back(j);
                                    }
                                }
                            }
                        }
                        else{//>
                            double compare=inq.mid(inq.indexOf(">")+1).toDouble();
                            inq.remove(inq.indexOf(">"),inq.size()-inq.indexOf(">"));
                            for(int j=0;j<pt->childCount();j++){
                                for(int k=0;k<pt->child(j)->childCount();k++){
                                    QString tem=pt->child(j)->child(k)->text(0);
                                    if(tem.indexOf(inq)!=-1&&tem.mid(tem.indexOf(":")+1).toDouble()>compare){
                                        r2.push_back(j);
                                    }
                                }
                            }
                        }
                    }
                    else if(inq.indexOf("<")!=-1){
                        int loc=inq.indexOf("<");
                        if(inq[loc+1].toLatin1()==61){//<=
                            double compare=inq.mid(inq.indexOf("=")+1).toDouble();
                            inq.remove(inq.indexOf("<"),inq.size()-inq.indexOf("<"));
                            for(int j=0;j<pt->childCount();j++){
                                for(int k=0;k<pt->child(j)->childCount();k++){
                                    QString tem=pt->child(j)->child(k)->text(0);
                                    if(tem.indexOf(inq)!=-1&&tem.mid(tem.indexOf(":")+1).toDouble()<=compare){
                                        r2.push_back(j);
                                    }
                                }
                            }
                        }
                        else{//<
                            double compare=inq.mid(inq.indexOf("<")+1).toDouble();
                            inq.remove(inq.indexOf("<"),inq.size()-inq.indexOf("<"));
                            for(int j=0;j<pt->childCount();j++){
                                for(int k=0;k<pt->child(j)->childCount();k++){
                                    QString tem=pt->child(j)->child(k)->text(0);
                                    if(tem.indexOf(inq)!=-1&&tem.mid(tem.indexOf(":")+1).toDouble()<compare){
                                        r2.push_back(j);
                                    }
                                }
                            }
                        }
                    }
                    else{//==
                        int loc=inq.indexOf("=");
                        QString compare=inq.mid(inq.indexOf("=")+2);
                        if(compare[0].toLatin1()==34){compare.remove(0,1);compare.remove(compare.size()-1,1);}
                        inq.remove(inq.indexOf("="),inq.size()-inq.indexOf("="));
                        for(int j=0;j<pt->childCount();j++){
                            for(int k=0;k<pt->child(j)->childCount();k++){
                                QString tem=pt->child(j)->child(k)->text(0);
                                QString te=tem.mid(tem.indexOf(":")+1);
                                if(te[0].toLatin1()==34){te.remove(0,1);te.remove(te.size()-1,1);}
                                if(tem.indexOf(inq)!=-1&&te==compare){
                                    r2.push_back(j);
                                }
                            }
                        }
                    }
                    std::vector<int>res;
                    if(boolean){//与
                        for(int t=0;t<r1.size();t++){
                            for(int T=0;T<r2.size();T++){
                                if(r1[t]==r2[T])
                                    res.push_back(r1[t]);
                            }
                        }
                    }
                    else{//或
                        for(int t=0;t<r1.size();t++){
                            bool similar=false;
                            for(int T=0;T<r2.size();T++){
                                if(r1[t]==r2[T])
                                    similar=true;
                            }
                            if(!similar)
                                res.push_back(r1[t]);
                        }
                        for(int t=0;t<r2.size();t++)
                            res.push_back(r2[t]);
                    }
                    for(int t=0;t<res.size();t++){
                        for(int T=0;T<pt->child(res[t])->childCount();T++){
                            if(tail.size()==0){
                                output+=pt->child(res[t])->child(T)->text(0);
                                output+="\r\n";
                            }
                            else if(pt->child(res[t])->child(T)->text(0).indexOf(tail)!=-1){
                                output+=pt->child(res[t])->child(T)->text(0);
                                output+="\r\n";
                            }
                        }
                    }
                }
            }
        }
    }
    ui->plainTextEdit_2->setPlainText(output);
}
