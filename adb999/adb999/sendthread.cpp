#include "sendthread.h"

sendthread::sendthread(QObject *parent) : QObject(parent)
{
    mfind =  adb_find();
    AppPath = mfind.GetAPPPath();


   // qDebug()<<"sdda444444: "+AppPath;
}

sendthread::~sendthread()
{


}

sendthread::SendApp(QString ip, QString info)
{

    //   qDebug()<<"info:"<<info;
    //   qDebug()<<"infoip:"<<ip;

       QByteArray ba = ip.toLatin1();
       char *ip_addr=ba.data();


       char*  sendData;
       ba = info.toLatin1();
       sendData=ba.data();


       WORD socketVersion = MAKEWORD(2,2);
       WSADATA wsaData;
       if(WSAStartup(socketVersion, &wsaData) != 0)
       {
           return 0;
       }

       SOCKET sclient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

       struct sockaddr_in sin;
       sin.sin_family = AF_INET;
       sin.sin_port = htons(12345);
       sin.sin_addr.S_un.S_addr = inet_addr(ip_addr);
       int len = sizeof(sin);

      // char * sendData = "GiveMeIp";

       sendto(sclient, sendData, strlen(sendData), 0, (struct sockaddr *)&sin, len);

       qDebug()<<"发送完毕";

       closesocket(sclient);
       WSACleanup();
       return 0;


}


void sendthread::SendCmd(QString cmd, int flag)
{

    qDebug()<<cmd+"  "+QString::number(flag,10);



     qDebug()<<"lllllllllllllllllllllllllll";

    QProcess adbProcess;
    QString strCmd =AppPath+cmd;



    adbProcess.start( strCmd);

    if(!adbProcess.waitForFinished(-1))
    {
      ret.clear();
    }
    else
    {
        if(flag == 1)   //标准信息
        {
           ret =QString::fromLocal8Bit(adbProcess.readAllStandardOutput());
        }
        else    //错误信息
        {
            ret =QString::fromLocal8Bit(adbProcess.readAllStandardError());

        }


    }
    ret += "ret";




   // adbProcess.waitForFinished();
   // adbProcess.finished();
}
