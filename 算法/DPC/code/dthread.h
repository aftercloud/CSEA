#ifndef DTHREAD_H
#define DTHREAD_H
#include<QThread>

class DThread : public QThread
{
    Q_OBJECT
public:
    DThread();
    void set(QString s);
    void run();

signals:
    void value(int);

private:
    QString file_name;
};

#endif // DTHREAD_H
