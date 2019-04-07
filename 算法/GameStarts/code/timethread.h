#ifndef TIMETHREAD_H
#define TIMETHREAD_H
#include<QThread>


class TimeThread : public QThread
{
    Q_OBJECT
public:
    TimeThread();
    void run();
    void set();

signals:
    void value(int);

private:
    bool mark;
};

#endif // TIMETHREAD_H
