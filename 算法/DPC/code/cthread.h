#ifndef CTHREAD_H
#define CTHREAD_H
#include<QThread>

class CThread : public QThread
{
    Q_OBJECT
public:
    CThread();
    void set(QString s);
    void run();

signals:
    void value(int);

private:
    QString file_name;

private:
    int charToSize(char target);
};

#endif // CTHREAD_H
