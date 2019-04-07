#ifndef BGTHREAD_H
#define BGTHREAD_H
#include<QThread>
#include<QtMultimedia/QSoundEffect>

class BGthread : public QThread
{
    Q_OBJECT
public:
    BGthread();
    void run();

private:
    QSoundEffect effect;

};

#endif // BGTHREAD_H
