#include "mythread.h"

MyThread::MyThread(QObject *parent) : QObject(parent)
{
    FrameCount =0;
    playstate = false;
    ThreadFind =   adb_find();
}

MyThread::~MyThread()
{

}

void MyThread::Screenshot()
{

    QString cmd,ret,downroad,filename;
    cmd = "adb shell screencap -p /sdcard/1.jpg";
    ret = ThreadFind.SendCmd(cmd,1);
  //  qDebug()<<"cmd:"+cmd;

    if(ret.lastIndexOf("successfully")>0)
    {
      downroad = "/sdcard/1.jpg";
      filename = QDir::currentPath()+"/temp.jpg";

      cmd = "adb pull "+downroad+" "+filename;
   //  qDebug()<<"shoot screen："+cmd;

      ret = ThreadFind.SendCmd(cmd,0);

      if(ret.indexOf("KB")>0)
      {

          QPixmap src(filename);//QPixmap src;   src.load(fileName);
          map = src;
          emit Start_Screenshot();
      // ui->pic_display->setPixmap(map.scaled(ui->pic_display->size()));
      }
      else
      {
           qDebug()<<"保存错误";
      }
    }
    else
    {
       qDebug()<<"failer:  "+cmd;
    }
}

void MyThread::Player(bool flag)
{
    FrameCount = 0;
    while(true)
    {
        Screenshot();
        QThread::msleep(100);
        FrameCount++;
        if(playstate) break;
      //qDebug()<<"FrameCount: "+QString::number(FrameCount,10);
    }
}
