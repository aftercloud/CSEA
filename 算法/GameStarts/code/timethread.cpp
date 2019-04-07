#include "timethread.h"
#include <QTime>

TimeThread::TimeThread()
{
    mark = true;
}

void TimeThread::run()
{
    int i = 0;
    while(mark)
    {
        QTime temp = QTime::currentTime().addSecs(1);
        while(temp >= QTime::currentTime());
        i++;
        emit value(i);
    }
    mark = true;
}

void TimeThread::set()
{
    this->mark = false;
}

