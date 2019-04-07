#ifndef DTHREAD_H
#define DTHREAD_H
#include<QThread>
#include<vector>

class DThread : public QThread
{
    Q_OBJECT
public:
    DThread();
    void set(QString s);
    void select(int on);
    void search(QString s);
    QStringList get_result();
    void inds();
    void run();
signals:
    void analyse(int);
private:
    QStringList word;//记录单词
    std::vector<QStringList*>link;//记录单词下的链接
    std::vector<int>where;
    QStringList net;//记录网址
    std::vector<double>num;//记录网址出现的次数
    QString file_name;
    int target;
    QString inquire;
    QStringList result;
    QString origin;
    QString ind;
    bool is_ind;
};

#endif // DTHREAD_H
