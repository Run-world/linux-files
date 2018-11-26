#include "kmsgthread.h"
#include "ui_mainwindow.h"

kmsgthread::kmsgthread(QObject *parent) : QObject(parent)
{
    status = false;
    kmsg_find = new adb_find();
  //  log_adb =  adb_find();
}

kmsgthread::~kmsgthread()
{

}

void kmsgthread::PrintKmsg(QString fileroute,QString filename)
{
    QString cmd = NULL;
    char*  cmd1;
    QString apppath = kmsg_find->GetAPPPath();
    if(filename.isEmpty())
    {
        filename = "kmsg";
    }
    cmd = apppath+"adb.exe shell \"dmesg\" > /sdcard/" + filename;


    // cmd = apppath+"adb.exe shell \"cat /proc/kmsg\" > /sdcard/" + filename;

    qDebug()<<"dmesg:  "+cmd;
    QByteArray temp = cmd.toLatin1();
    cmd1 = temp.data();
    WinExec(cmd1,SW_HIDE);
    QThread::msleep(1000);
    cmd = "adb pull /sdcard/"+ filename+ " " +fileroute;

    qDebug()<<"dmesg cmd:  "+cmd;

    while(!status)
    {
        kmsg_find->SendCmd(cmd,1);
        qDebug()<<"导出成功dmesg";
        QThread::msleep(2000);
    }
//    cmd = "adb shell rm /sdcard/"+ filename;
//    kmsg_find->SendCmd(cmd,1);
//    qDebug()<<"删除成功";
}
