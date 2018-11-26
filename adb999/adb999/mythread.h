#ifndef MYTHREAD_H
#define MYTHREAD_H

#include "adb_find.h"
#include <QObject>
#include  <winsock2.h>
#include <QDebug>
#include <QPixmap>
#include <QImage>
#include <QFileDialog>
#include <QThread>

#pragma comment(lib, "ws2_32.lib")


class MyThread : public QObject
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = 0);
    ~MyThread();
    void Screenshot();
   // void PrintLogCat();
    QPixmap map;
    int FrameCount;
    bool playstate;



signals:

    void Start_Screenshot();

public slots:
    void Player(bool);

private:
    adb_find ThreadFind;

};




#endif // MYTHREAD_H
