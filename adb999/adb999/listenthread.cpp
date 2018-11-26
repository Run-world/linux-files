#include "listenthread.h"
#include <QByteArray>
#include <iostream>
#include <qDebug>
#include <QProcess>
#include <QThread>


listenthread::listenthread(QObject *parent) : QObject(parent)
{

    BroadcatIpList.clear();
    ipadress.clear();
    stop = false;
    RetStr.clear();

    uSocket = new QUdpSocket;

    uSocket->bind(12345,QUdpSocket::ShareAddress);
    qDebug()<<"dada2222";

    connect(uSocket, SIGNAL(readyRead()), this, SLOT(ListenAPK()));

}

listenthread::~listenthread()
{

}

QStringList listenthread::GetIpList(QString myip)
{

     QStringList list;
     QString  ret;

     int Entercount=0;
     int end;

    //QThread::msleep(100);

 //   qDebug()<<"dd11: "+QString::number(Entercount,10);


     for(int i=0; i<BroadcatIpList.size();i++)
     {
         qDebug()<<list.at(i);
     }

     return list;

}

QString listenthread::SendCmd(QString cmd)
{
    QString str;
    QProcess adbProcess;
    QString strCmd = "C:\\Windows\\SysWOW64\\"+cmd;



    adbProcess.start( strCmd);

    if(!adbProcess.waitForFinished(-1))
    {
       return NULL;
    }
    else
    {
       str =QString::fromLocal8Bit(adbProcess.readAllStandardOutput());
       return str;
    }

}

int listenthread::ListenAPK()
{

   // qDebug()<<"dadasd";

    QHostAddress client_address;
    QByteArray ba;
    while(uSocket->hasPendingDatagrams())
    {
        ba.resize(uSocket->pendingDatagramSize());
        uSocket->readDatagram(ba.data(), ba.size(),&client_address);
        QString recv = QString(ba);
      //  qDebug()<<"recv: "+recv+client_address.toString();
        RetStr =  recv;

        if(recv.compare("Unionman")==0)
        {

            ipadress =  client_address.toString();

            if(BroadcatIpList.indexOf(ipadress)<0)
            {
                if(ipadress.compare(localIp)!=0)
                  BroadcatIpList<<ipadress;
            }
        }

        // std::cout << ba.data() << std::endl;
    }









}

