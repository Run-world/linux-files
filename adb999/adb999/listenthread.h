#ifndef LISTENTHREAD_H
#define LISTENTHREAD_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QObject>
#include <QThread>
#include <QUdpSocket>
#include <QByteArray>
#include <QStringList>


#pragma comment(lib, "ws2_32.lib")
#include  <winsock2.h>

class listenthread : public QObject
{
    Q_OBJECT
public:
    explicit listenthread(QObject *parent = 0);
    ~listenthread();
    QString RetStr;
    QString ipadress;
    SOCKET serSocket;
    QStringList BroadcatIpList;
    QStringList GetIpList(QString myip);
    QString SendCmd(QString cmd);
     QString localIp;

    bool stop;

signals:

public slots:

    int ListenAPK(void);
private:
    QUdpSocket *uSocket;
};


#endif // LISTENTHREAD_H
