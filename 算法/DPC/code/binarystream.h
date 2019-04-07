#ifndef BINARYSTREAM_H
#define BINARYSTREAM_H
#include<QVector>

class binaryStream
{
    int mark;
public:
    binaryStream();
    void bs(QVector<char>&win,char tar,int len,bool bc = false);
    int is(QVector<char>&res,char*p,int&cnt);
};

#endif // BINARYSTREAM_H
