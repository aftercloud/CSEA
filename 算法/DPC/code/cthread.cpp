#include "cthread.h"
#include<QFile>
#include<QDataStream>
#include<QVector>
#include<QLatin1String>
#include<QByteArray>

#include"binarystream.h"
#include"huffmantree.h"

#define MAX 2147000000

CThread::CThread()
{
    this->file_name = "";
}

void CThread::set(QString s)
{
    this->file_name = s;
}

int CThread::charToSize(char target)
{
    int tar = (unsigned char)target;
    if(tar <= 1)return 1;
    int count = 1;
    while(tar / 2 >= 1)
    {
        count++;
        tar /= 2;
    }
    return count;
}

void CThread::run()
{
    QString open_name = this->file_name;
    QFile file_in(open_name);
    if(file_in.open(QIODevice::ReadOnly))
    {
        file_in.seek(18);
        QString save_name = open_name;
        save_name.remove(save_name.length()-4,4);
        save_name += ".img";
        this->file_name = save_name;
        QFile file_out(save_name);
        if(file_out.open(QIODevice::WriteOnly))
        {
            QDataStream in(&file_in);
            QDataStream out(&file_out);
            char*rect = new char[4];
            in.readRawData(rect,4);
            int width = 0;
            width += ((unsigned char)rect[3])*16777216;
            width += ((unsigned char)rect[2])*65535;
            width += ((unsigned char)rect[1])*256;
            width += (unsigned char)rect[0];
            in.readRawData(rect,4);
            int height = 0;
            height += ((unsigned char)rect[3])*16777216;
            height += ((unsigned char)rect[2])*65535;
            height += ((unsigned char)rect[1])*256;
            height += (unsigned char)rect[0];
            file_in.seek(10);
            in.readRawData(rect,4);
            int off = 0;
            off += ((unsigned char)rect[3])*16777216;
            off += ((unsigned char)rect[2])*65535;
            off += ((unsigned char)rect[1])*256;
            off += (unsigned char)rect[0];
            delete[] rect;
            file_in.seek(0);
            int size = file_in.size()-off;
            rect = new char[off];
            char*nums = new char[size];
            in.readRawData(rect,off);
            in.readRawData(nums,size);
            out << size;
            out.writeRawData(rect,off);
            QVector<int>l(1,0);
            QVector<int>b(1,0);
            for(int i=0;i<size;i++)
            {
                int len = charToSize(nums[i]);
                int worked = 0;
                for(int j=i;j<size&&j<i+256;j++)
                {
                    if(charToSize(nums[j]) == len)
                        worked++;
                    else break;
                }
                i += (worked - 1);
                l.push_back(len);
                b.push_back(worked);
            }
            size = l.size() -1;
            QVector<int>trace(2,0);
            QVector<int>back(2,0);
            QVector<int>bsum(2,0);
            QVector<int>s(2,0);
            trace[1] = 1;
            back[1] = l[1];
            bsum[1] = b[1];
            s[1] = l[1] * b[1] + 11;
            for(int i=2;i<=size;i++)
            {
                s.append(MAX);
                int index = 0;
                int bmax = 0;
                int nus = 0;
                int limit = 256 > i?i:256;
                for(int k =1;k<=limit;k++)
                {
                    int temp_sum = 0;
                    int temp_max = 0;
                    for(int g=i;g>i-k;g--)
                    {
                        temp_sum += b[g];
                        if(temp_sum > 256)
                            break;
                        if(temp_max<l[g])
                            temp_max = l[g];
                    }
                    if(temp_sum>256)
                        break;
                    if(s[i]>s[i-k]+temp_max*temp_sum+11)
                    {
                        s[i] = s[i-k] + temp_max * temp_sum + 11;
                        index = k;bmax = temp_max;nus = temp_sum;
                    }
                }
                bsum.append(nus);
                back.append(bmax);
                trace.append(index);
            }
            emit value(2);
            int count = 0;
            for(int i=trace.size()-1;i>0;i--)
            {
                count++;
                int temp = trace[i];
                for(int j=i-1;j>i-temp&&j>=0;j--)
                    trace[j] = -1;
                i -= (temp - 1);
            }
            QVector<int>res;
            QVector<int>lr;
            for(int i=1;i<trace.size();i++)
            {
                if(trace[i]!=-1)
                {
                    res.append(bsum[i]);
                    lr.append(back[i]);
                }
            }
            size = height * width;
            QVector<char>win(1,0);
            binaryStream sr;
            for(int i=0,j=0;i<size;){
                int len = lr[j];
                int worked = res[j++];
                sr.bs(win,(char)len,3,true);
                sr.bs(win,(char)worked,8);
                for(int k=0;k<worked;k++)
                {
                    sr.bs(win,nums[i+k],len);
                }
                i += worked;
                if(i%3000==0){
                    int me = (int)((5+i/1050/size)/2);
                    emit value(me);
                }
            }
            for(int i=0;i<win.size();i++){
                out.writeRawData(&win[i],1);
                if(i%3000==0){
                    int me = (int)((40+i/1800/size)/2);
                    emit value(me);
                }
            }
            file_out.close();
            file_in.close();
        }
    }
    emit value(50);
    {
        QString file_name = this->file_name;
        QFile file(file_name);//选定任意文件
        if(file.open(QIODevice::ReadOnly))//如果成功打开
        {if(file_name.right(3)!="huf"){//压缩文件
                QDataStream in(&file);//从file中读取
                int tai=file_name.toLocal8Bit().indexOf(".");
                QString tail;
                if(tai!=-1){
                    tail=file_name.right(file_name.toLocal8Bit().length()-tai-1);
                }
                else tail="0";
                char*s=0;//数据读入s
                s=new char[1024];
                int file_size=file.size();//获取文件长度
                int read_num=file_size/1024;//读取文件次数
                int huff[256];//编码有2^8=256种可能
                for (int i = 0; i < 256; i++) {//统计初始化
                    huff[i] = 0;
                }
                int temp = 0;
                for(int i=0;i<read_num;i++){
                    in.readRawData(s,1024);//读取
                    for (int j = 0; j < 1024; j++) {//获取0~255的ASCII码
                        temp = (int)s[j];//在相应位置累计
                        if(temp<0)temp+=256;
                        huff[temp]++;
                    }
                }
                file_size%=1024;
                in.readRawData(s,file_size);//读取
                for (int j = 0; j < file_size; j++) {
                    temp = (int)s[j];
                    if(temp<0)temp+=256;
                    huff[temp]++;
                }//编码统计完成
                int huff_str_num=0;
                for (int i = 0; i < 256; i++) {
                    if(huff[i]!=0)huff_str_num++;
                }//编码种类
                delete[]s;
                file.close();
                HuffmanTree<int>h(huff,huff_str_num);//生成哈夫曼树
                QString*qt=new QString[huff_str_num];
                s=new char[huff_str_num];
                int num=0;QString qs="";
                h.output(h.root,qt,s,num,qs);//获取哈夫曼编码
                QString bi;//存储二进制字符串
                QString bin;//转换二进制字符串
                bool ok;char*ci=new char[1024];//写入
                if(file.open(QIODevice::ReadOnly))//开始转码
                {
                    QFile fi;
                    QString fi_name;
                    int locat=file_name.indexOf(".");
                    if(locat!=-1)fi_name=file_name.left(locat);
                    else fi_name=file_name;
                    fi_name+=".huf";
                    fi.setFileName(fi_name);
                    fi.open(QIODevice::WriteOnly);//打开压缩文件写入
                    QDataStream out(&fi);
                    QDataStream ii(&file);
                    char*c=new char[1024];
                    file_size=file.size();//获取文件长度
                    QString config=QString::number(file_size);//写入原文件字节数
                    config+=" ";//以空格作为分隔符
                    config+=tail;//写入文件后缀
                    config+=" ";
                    config+=QString::number(huff_str_num);//写入哈夫曼树大小
                    config+=" ";
                    for(int i=0;i<256;i++){//依次写入哈夫曼编码
                        config+=QString::number(huff[i]);
                        config+=" ";
                    }
                    int huff_size=config.toLocal8Bit().length();
                    QString ts;
                    ts=QString::number(huff_size)+" "+config;
                    int huff_si=ts.toLocal8Bit().length();
                    QByteArray by=ts.toLatin1();
                    char*cs=by.data();
                    out.writeRawData(cs,huff_si);
                    read_num=file_size/1024;//读取文件次数
                    int va=0,y=0;
                    for(int i=0;i<read_num;i++){
                        ii.readRawData(c,1024);//每次读取1KB
                        va+=1024;
                        if(i%3000==0){
                            int me = (int)(45+((40+i/3010/read_num)/2));
                            emit value(me);
                        }
                        for(int j=0;j<1024;j++){//转码
                            for(int k=0;k<huff_str_num;k++){//匹配
                                if(c[j]==s[k]){
                                    bi+=qt[k];
                                    while(bi.length()>=8){
                                        bin=bi.left(8);
                                        bi.remove(0,8);
                                        ci[y++]=bin.toInt(&ok,2);
                                        if(y==1024){
                                            y=0;
                                            out.writeRawData(ci,1024);//若bi中还有剩余，再做处理
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                    }
                    file_size%=1024;
                    ii.readRawData(c,file_size);//读取不足1KB的部分
                    for(int j=0;j<file_size;j++){
                        for(int k=0;k<huff_str_num;k++){//匹配
                            if(c[j]==s[k]){
                                va++;
                                bi+=qt[k];
                                while(bi.length()>=8){
                                    bin=bi.left(8);
                                    bi.remove(0,8);
                                    ci[y++]=bin.toInt(&ok,2);
                                    if(y==1024){
                                        y=0;
                                        out.writeRawData(ci,1024);//若bi中还有剩余，再做处理
                                    }
                                }
                                break;
                            }
                        }
                    }
                    while(bi.length()!=8){
                        bi+="0";
                    }
                    ci[y++]=bi.toInt(&ok,2);
                    out.writeRawData(ci,y);
                    delete[]c;
                    file.close();fi.close();
                }
                delete ci;
                delete[]s,qt;
            }
            else{//解压
                int huff_size=0;
                QDataStream in(&file);
                char ss=0;
                in.readRawData(&ss,1);
                huff_size+=(ss-48);
                in.readRawData(&ss,1);
                for(int i=0;ss!=' ';i++){
                    huff_size*=10;
                    huff_size+=(ss-48);
                    in.readRawData(&ss,1);
                }
                char*s=new char[huff_size];
                in.readRawData(s,huff_size);
                QLatin1String te(s);
                QString config(te);
                delete[]s;
                int locat=config.indexOf(" ");
                int huff_s=config.left(locat).toInt();//原始文件大小
                config.remove(0,locat+1);
                locat=config.indexOf(" ");
                QString tail=config.left(locat);//文件后缀
                config.remove(0,locat+1);
                locat=config.indexOf(" ");
                int huff_c=config.left(locat).toInt();//哈夫曼树大小
                config.remove(0,locat+1);
                bool ok;
                int huff[256];
                for(int i=0;i<256;i++){
                    huff[i]=0;
                }
                for(int i=0;i<256;i++){
                    locat=config.indexOf(" ");
                    huff[i]=config.left(locat).toInt();
                    config.remove(0,locat+1);
                }//哈夫曼树数据加载完成
                HuffmanTree<int>h(huff,huff_c);//重新生成哈夫曼树
                file_name.remove(file_name.length()-3,3);
                if(tail!="0"){
                    int tai=file_name.indexOf(".");
                    file_name.remove(tai+1);
                    file_name+=tail;
                }
                QFile fi(file_name);
                fi.open(QIODevice::WriteOnly);
                QDataStream out(&fi);
                char*c=new char[1024];
                HuffmanNode<int>*leaf=h.root;
                int Num=huff_s;
                int i=0,y=0;//计数
                char*u=new char[1024];
                while(1){
                    in.readRawData(c,1024);
                    for(int j=0;j<1024;j++){
                        for(int k=0;k<8;k++){
                            if((unsigned int)c[j]<128){//进左孩子树
                                leaf=leaf->leftChild;
                            }
                            else{//进右孩子树
                                leaf=leaf->rightChild;
                            }
                            c[j]*=2;//位操作
                            if(leaf->leftChild==0){//如果是叶节点
                                u[y++]=leaf->huf;//从节点leaf获取数据huf
                                if(y==1024){
                                    y=0;
                                    out.writeRawData(u,1024);//写入文件
                                }
                                leaf=h.root;//重置leaf指针
                                i++;//写入数据总量
                                if(i>=Num)break;//当写入数据总量
                            }
                        }
                        if(i>=Num)break;
                    }
                    if(i>=Num)break;
                    if(i%3000==0){
                        int me = (int)(50+((i/3000/Num)/2));
                        emit value(me);
                    }
                }
                out.writeRawData(u,y);
                delete[]c;
                delete[]u;
                file.close();fi.close();
            }
        }
    }
    emit value(100);
}
