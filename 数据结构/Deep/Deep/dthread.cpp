#include "dthread.h"
#include"mainwindow.h"
#include<QFile>
#include<QTextStream>

DThread::DThread():word(),link(),num(),net(),result(),where()
{
    file_name=inquire=origin=ind="";
    target=true;is_ind=false;
}

void DThread::run()
{
    QFile file(file_name);
    if(target==0)//文件
    {
        if(file.open(QIODevice::ReadWrite)){
            QFile File("index.txt");
            if(File.open(QIODevice::WriteOnly)){
                QTextStream out(&file);
                QTextStream in(&File);
                out.seek(file.size());
                out<<"\r\n";
                out.seek(0);
                int index_count=0;
                int accumulate=0;
                int link_num=0;
                std::vector<int>page_link;
                emit analyse(file.size());
                int emit_count=0;
                QString read="";
                word.clear();//记录单词
                bool ign=false;
                QStringList*real=0;
                if(!link.empty())
                {
                    for(int i=0;i<link.size();i++)
                    {
                        real=link.at(i);
                        delete real;
                    }
                }
                link.clear();//记录单词下的链接
                net.clear();//记录网址
                num.clear();//记录网址出现的次数
                where.clear();//记录原始文件中网页的位置
                QString temp1="",temp2="";int locate=0;
                while(!out.atEnd())
                {
                    read=out.readLine();
                    if(ign&&read.isEmpty())
                    {
                        accumulate+=2;ign=false;
                        double vote=(double)1/link_num;
                        for(int i=0;i<page_link.size();i++)
                            num[page_link[i]]+=vote;
                        page_link.clear();
                        link_num=0;
                        continue;
                    }
                    switch (read[0].toLatin1()) {
                    case 80://P
                        ign=true;
                        read.remove(0,2);
                        locate=net.indexOf(read);
                        if(locate==-1)//如果链接未存在
                        {
                            net<<read;
                            index_count++;
                            num.push_back(0);
                            where.push_back(accumulate);
                        }
                        else//如果链接已存在
                            where[locate]=accumulate;
                        break;
                    case 76://L
                        read.remove(0,2);
                        locate=net.indexOf(read);
                        link_num++;
                        if(locate==-1){//如果链接未存在
                            net<<read;
                            index_count++;
                            num.push_back(0);
                            page_link.push_back(index_count-1);
                            where.push_back(-1);
                        }
                        else{//如果链接已存在
                            page_link.push_back(locate);
                        }
                        break;
                    case 84://T
                        read.remove(0,2);
                        break;
                    default://Q
                        read.remove(0,2);
                        for(int i=0;i<read.count(' ');i++)
                        {
                            temp1=read.section(' ',i,i);
                            locate=word.indexOf(temp1);
                            if(locate!=-1)//如果单词有记录
                            {
                                real=link[locate];
                                temp2=QString::number(index_count-1);
                                if(!real->contains(temp2)){//如果网址未被记载
                                    real->operator <<(temp2);
                                }
                            }
                            else//如果单词无记录
                            {
                                word<<temp1;
                                real=0;real=new QStringList;
                                real->operator <<(QString::number(index_count-1));
                                link.push_back(real);
                            }
                        }
                        break;
                    }

                    accumulate+=(read.size()+4);//计算换行符
                    emit_count++;
                    if(emit_count==3600)
                    {
                        emit_count=0;
                        emit analyse(accumulate);
                    }
                }
                emit_count=0;accumulate=0;
                emit analyse(file.size());
                emit analyse(net.size()+word.size());
                in<<file_name<<"\r\n";
                for(int i=0;i<net.size();i++){
                    in<<net.at(i)<<"\t"<<QString::number(num[i])<<"\t"<<QString::number(where[i])<<"\r\n";
                    accumulate++;emit_count++;
                    if(emit_count==1000)
                    {
                        emit_count=0;
                        emit analyse(accumulate);
                    }
                }
                in<<"\r\n";
                for(int i=0;i<word.size();i++)
                {
                    in<<word.at(i)<<"\t";
                    real=link[i];
                    in<<real->join("\t")<<"\r\n";
                    delete real;
                    accumulate++;emit_count++;
                    if(emit_count==1000)
                    {
                        emit_count=0;
                        emit analyse(accumulate);
                    }
                }
                link.clear();
                emit analyse(accumulate);
                File.close();
            }
            file.close();
        }
        else return;
    }
    else if(target==-1)//索引
    {
        if(file.open(QIODevice::ReadOnly))
        {
            is_ind=true;
            ind=file_name;
            QTextStream out(&file);
            origin=out.readLine();
            QString read="";
            QStringList*real=0;bool change=false;
            QString temp1="",temp2="";int locate=0;
            int index_count=0;
            int accumulate=origin.size();int add=0;
            word.clear();//记录单词
            //link需要先delete
            if(!link.empty())
            {
                for(int i=0;i<link.size();i++)
                {
                    real=link.at(i);
                    delete real;
                }
            }
            link.clear();//记录单词下的链接
            net.clear();//记录网址
            num.clear();//记录网址出现的次数
            where.clear();
            emit analyse(file.size());
            int emit_count=0;
            while(!out.atEnd())
            {
                read=out.readLine();
                add=read.size();
                accumulate+=add;
                if(add==0)
                {
                    change=true;
                    continue;
                }
                if(!change)//加载网页字典
                {
                    net<<read.section('\t',0,0);
                    num.push_back(read.section('\t',1,1).toDouble());
                    where.push_back(read.section('\t',2,2).toInt());
                }
                else//加载倒排索引
                {
                    word<<read.section('\t',0,0);
                    real=0;
                    real=new QStringList;
                    for(int i=1;i<=read.count('\t');i++)
                        real->operator <<(read.section('\t',i,i));
                    link.push_back(real);
                }
                emit_count++;
                if(emit_count==20)
                {
                    emit_count=0;
                    emit analyse(accumulate);
                }
            }
            emit analyse(-1);//初始化主窗体辅助变量
            result.clear();
            for(int i=0;i<num.size();i++)
            {
                if(num[i]>=2)
                    result<<net.at(i);
                if(result.size()==10)break;
            }
            file.close();
        }
        else return;
    }
    else if(target==-2)//增量
    {
        QFile File(origin);
        if(file.open(QIODevice::ReadOnly)&&File.open(QIODevice::ReadWrite))
        {
            QTextStream out(&file);
            QTextStream in(&File);
            int locate=File.size();
            in.seek(locate);
            in<<"\r\n\r\n";
            QString read="";
            while(!out.atEnd())
            {
                read=out.readLine()+"\r\n";
                in<<read;
            }
            File.close();
            File.open(QIODevice::ReadOnly);
            in.seek(locate);
            file.close();
            file.setFileName("index.txt");
            file.open(QIODevice::ReadOnly);
            QTextStream ge(&file);
            file_name=ge.readLine();
            file.close();
            if(file.open(QIODevice::WriteOnly))
            {
                QTextStream inn(&file);//重写索引
                QString read="";
                while(in.readLine().isEmpty())
                    locate+=2;
                in.seek(locate);
                int index_count=net.size();
                int accumulate=locate;
                int link_num=0;
                std::vector<int>page_link;
                emit analyse(File.size());
                int emit_count=0;
                bool ign=false;
                QStringList*real=0;
                QString temp1="",temp2="";
                while(!in.atEnd())
                {
                    read=in.readLine();
                    if(ign&&read.isEmpty())
                    {
                        accumulate+=2;ign=false;
                        double vote=(double)1/link_num;
                        for(int i=0;i<page_link.size();i++)
                            num[page_link[i]]+=vote;
                        page_link.clear();
                        link_num=0;
                        continue;
                    }
                    switch (read[0].toLatin1()) {
                    case 80://P
                        ign=true;
                        read.remove(0,2);
                        locate=net.indexOf(read);
                        if(locate==-1)//如果链接未存在
                        {
                            net<<read;
                            index_count++;
                            num.push_back(0);
                            where.push_back(accumulate);
                        }
                        else//如果链接已存在
                            where[locate]=accumulate;
                        break;
                    case 76://L
                        read.remove(0,2);
                        locate=net.indexOf(read);
                        link_num++;
                        if(locate==-1){//如果链接未存在
                            net<<read;
                            index_count++;
                            num.push_back(0);
                            page_link.push_back(index_count-1);
                            where.push_back(-1);
                        }
                        else{//如果链接已存在
                            page_link.push_back(locate);
                        }
                        break;
                    case 84://T
                        read.remove(0,2);
                        break;
                    default://Q
                        read.remove(0,2);
                        for(int i=0;i<read.count(' ');i++)
                        {
                            temp1=read.section(' ',i,i);
                            locate=word.indexOf(temp1);
                            if(locate!=-1)//如果单词有记录
                            {
                                real=link[locate];
                                temp2=QString::number(index_count-1);
                                if(!real->contains(temp2)){//如果网址未被记载
                                    real->operator <<(temp2);
                                }
                            }
                            else//如果单词无记录
                            {
                                word<<temp1;
                                real=0;real=new QStringList;
                                real->operator <<(QString::number(index_count-1));
                                link.push_back(real);
                            }
                        }
                        break;
                    }

                    accumulate+=(read.size()+4);//计算换行符
                    emit_count++;
                    if(emit_count==3600)
                    {
                        emit_count=0;
                        emit analyse(accumulate);
                    }
                }
                emit_count=0;accumulate=0;
                emit analyse(File.size());
                emit analyse(net.size()+word.size());
                inn<<file_name<<"\r\n";
                for(int i=0;i<net.size();i++){
                    inn<<net.at(i)<<"\t"<<QString::number(num[i])<<"\t"<<QString::number(where[i])<<"\r\n";
                    accumulate++;emit_count++;
                    if(emit_count==1000)
                    {
                        emit_count=0;
                        emit analyse(accumulate);
                    }
                }
                inn<<"\r\n";
                for(int i=0;i<word.size();i++)
                {
                    inn<<word.at(i)<<"\t";
                    real=link[i];
                    inn<<real->join("\t")<<"\r\n";
                    accumulate++;emit_count++;
                    if(emit_count==1000)
                    {
                        emit_count=0;
                        emit analyse(accumulate);
                    }
                }
                emit analyse(accumulate);
                file.close();
            }
            else  return;
            File.close();
        }
        else return;
    }
    else if(target==-3)//搜索
    {
        result.clear();
        int key_num=inquire.count(' ')+1;
        QStringList key_list=inquire.split(' ');
        QStringList**tar=new QStringList*[key_num];
        for(int i=0;i<key_num;i++)
            tar[i]=link.at(word.indexOf(key_list.at(i)));
        double*order=new double[tar[0]->size()];//记录可信度
        bool mark=true;
        for(int i=0;i<tar[0]->size();i++)
            order[i]=0;
        QString isc="";
        std::vector<QString>f;//满足要求的网址进向量
        for(int i=0;i<tar[0]->size();i++)
        {
            isc=tar[0]->at(i);
            if(!is_ind){
            if(where[isc.toInt()]==-1)continue;}
            for(int j=1;j<key_num;j++)
            {
                if(!tar[j]->contains(isc))
                {
                    mark=false;
                    break;
                }
                mark=true;
            }
            if(mark)
                f.push_back(net.at(isc.toInt()));
        }
        if(f.size()==0)
        {
            delete[]order;
            delete[]tar;
            result<<"no page";
            emit analyse(2147000000);
            return;
        }
        for(int i=0;i<f.size();i++)
            order[i]=num[net.indexOf(f[i])];
        int t1=0;QString t2="";
        for(int i=0;i<f.size();i++)//排序
        {
            double max=order[i];
            for(int j=i+1;j<f.size();j++)
            {
                if(max<order[j])
                {
                    max=order[j];
                    t1=order[i];
                    order[i]=order[j];
                    order[j]=t1;
                    t2=f[i];
                    f[i]=f[j];
                    f[j]=t2;
                }
            }
        }
        for(int i=0;i<f.size();i++)
            result<<f[i];
        delete[]order;
        delete[]tar;
        emit analyse(2147000000);
    }
    else//获取
    {
        int page_num=this->target;
        int range=page_num*10>result.size()?result.size():page_num*10;
        page_num--;
        QString source="";
        file.setFileName(ind);
        if(file.open(QIODevice::ReadOnly))
        {
            QTextStream out(&file);
            source=out.readLine();
            file.close();
        }
        else return;
        QFile sh(source);
        if(sh.open(QIODevice::ReadOnly))
        {
            QTextStream out(&sh);
            QString re="";int se=0;
            for(int i=page_num*10;i<range;i++)
            {
                source=result.at(i);
                re+="P\t";re+=source;re+="\r\n";
                se=where[net.indexOf(result.at(i))];
                if(se==-1)
                {
                    re+="\r\n";
                    continue;
                }
                out.seek(se);
                do
                    source=out.readLine().remove(0,2);
                while(source!=result.at(i));
                do{
                    source=out.readLine();
                    re+=source;re+="\r\n";
                }while(!source.isEmpty());
                re+="\r\n";
            }
            sh.close();
            sh.setFileName("show.txt");
            if(sh.open(QIODevice::WriteOnly))
            {
                out.seek(0);
                out<<re;
                sh.close();
            }
            else return;
            emit analyse(2146000000);
        }
        else return;
    }
}

void DThread::set(QString s)
{
    file_name=s;
}

void DThread::search(QString s)
{
    inquire=s;
}

void DThread::select(int on)
{
    target=on;
}

QStringList DThread::get_result()
{
    return result;
}

void DThread::inds()
{
    is_ind=false;
}
