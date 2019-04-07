#include "binarystream.h"

binaryStream::binaryStream()
{
    mark = 0;
}

void binaryStream::bs(QVector<char>&win,char tar,int len,bool bc)
{
    int temp = 8 - mark;
    if(tar == 8 && bc)
        tar = 0;
    if(temp >= len)
    {
        int ls = temp - len;
        tar <<= ls;
        win[win.size()-1] |= tar;
        mark += len;
    }
    else
    {
        win[win.size()-1]|= ((unsigned char)tar)>>(len-temp);
        temp = 8 - len + temp;
        tar <<= temp;
        win.append(tar);
        mark = (mark + len) % 8;
    }
    if(mark == 8 || mark == 0)
    {
        win.append(0);
        mark = 0;
    }
}

int binaryStream::is(QVector<char> &res, char *p, int &cnt)
{
    int count = 0;
    int temp = 8 - mark;
    int len = 0;
    int bit = 0;
    if(temp >= 3)
    {
        len = (7 & ((unsigned char)*p) >> (temp - 3));
        if(len == 0)
            len = 8;
        mark += 3;
    }
    else
    {
        count++;
        if(temp == 1)
        {
            if(*p&1==1)
                len = 4;
            len += (((unsigned char)*(p+1))>>6) & 3;
            mark = 2;
        }
        else
        {
            len = 2* (((unsigned char)*p) & 3);
            if((((unsigned char)(*(p+1)))>>7)&1==1)
                len++;
            mark = 1;
        }
        if(len == 0)
            len = 8;
    }
    if(mark == 8)
    {
        bit = (unsigned char)*(p+1);
        if(bit == 0)
            bit = 256;
        mark=0;
        count+=2;
    }
    else
    {
        char a = ((unsigned char)*(p + count))<<mark;
        char b = (((unsigned char)*(p + count + 1))>>(8 - mark));
        bit = (unsigned char)a|b;
        if(bit == 0)
            bit = 256;
        count++;
    }
    for(int i=0;i<bit;i++)
    {
        char a = 0;
        char b = 0;
        char target = 0;
        int leng = 8 - mark;
        if(leng >= len)
        {
            char show = *(p+count);
            a = *(p + count)<<mark;
            b = ((unsigned char)a)>>(8-len);
            target = (unsigned char)b;
            mark += len;
            if(mark >=8)
            {
                mark %= 8;
                count++;
            }
        }
        else
        {
            char show = *(p+count);
            a = (((unsigned char)(*(p + count)<<mark))>>mark)<<(len-leng);
            mark = len - leng;
            b = ((unsigned char)*(p + count + 1))>>(8 - mark);
            target = (unsigned char)a|b;
            count++;
        }
        cnt++;
        res.append(target);
    }
    return count;
}
