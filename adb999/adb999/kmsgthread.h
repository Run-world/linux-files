#ifndef KMSGTHREAD_H
#define KMSGTHREAD_H


#include "adb_find.h"

#include <QDebug>
#include <QString>
#include <QThread>
#include <QObject>
#include  <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

class kmsgthread : public QObject
{
    Q_OBJECT
public:
    explicit kmsgthread(QObject *parent = 0);
    ~kmsgthread();
    bool status;

private:
    adb_find *kmsg_find;



signals:

public slots:
  void  PrintKmsg(QString fileroute,QString filename);



};

#endif // KMSGTHREAD_H
