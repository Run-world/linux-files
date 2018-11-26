#include "logthread.h"
#include <Qfile>
#include "ui_mainwindow.h"
#include <QStringList>


QProcess pro;

logthread::logthread(QObject *parent) : QObject(parent)
{
    PrintFlag = false;
    status = false;
    log_adb =  adb_find();




}

logthread::~logthread()
{

}

void logthread::LogProcessRound(QString cmd,QString filename)
{

     QStringList templist;
     qDebug()<<"apppathcmd :"+cmd+"kkk:"+filename;


      cmd  = cmd+" > "+filename+"\n";

   //  cmd  = cmd+" > "+"C:\/Users\/油腻man\/Desktop\/2018-08-17-08-30-15-195logcat.txt"+"\n";

     qDebug()<<"1cmd:"+cmd;
     QByteArray ba = cmd.toLocal8Bit(); // must
     char*  ch;
     ch = ba.data();

     pLogProcess = new QProcess();

  /////////////////////////////////////////////////
     pidlist.clear();
     QString ret =   log_adb.SendCmd("adb shell ps | grep logcat",1);

     pidlist = ret.split("root");
     int index;

     for(int i=0;i<pidlist.size();i++)
     {
         QString  temp = pidlist.at(i).trimmed().simplified();
         index  =  temp.indexOf(" ");
         temp = temp.left(index).trimmed();
         templist<<temp;
        // pidlist.at(i) =temp;
         qDebug()<<"111:"+ temp;
     }
     pidlist.clear();
     pidlist =  templist;


  //   printf("cmd222: %s",ch);

     emit GetPidSingal();



////////////////////////////////////////////////////////////

     //QProcess p(0);
     pLogProcess->start("cmd");

     pLogProcess->waitForStarted();
     pLogProcess->write(ch);

     pLogProcess->closeWriteChannel();

     pLogProcess->waitForFinished(-1);





     qDebug()<<"1234232546578";

///////////////////////////////////////////////////////
















}


void logthread::PrintLogCat(QString pid,QString cmd2,QString filename)
{



    QString cmd;


    QString apppath = log_adb.GetAPPPath();




        cmd = apppath+"adb.exe shell \"";
        if(cmd2.isEmpty())
        {
            cmd = apppath+"adb.exe shell \" logcat -c ; logcat -v time";
            qDebug()<<"框为空:  "+cmd;
        }
        else{
            qDebug()<<"框的值:  "+cmd2;
            cmd = cmd  + cmd2;
            qDebug()<<"框不为空:  "+cmd;
        }

        if(!pid.isEmpty())
        {
            cmd = cmd+" | grep "+pid;

        }

        cmd = cmd+"\" ";

        LogProcessRound(cmd,filename);


}

void logthread::StopLogSlot()
{
    qDebug()<<"stop111 log";

  //  pLogProcess->write("ctrl+c\n");


//    pLogProcess->write("\3");

 //   pLogProcess->waitForFinished(10);

//       pLogProcess->destroyed();

//    qDebug()<<"stop111 log111";



    if(pLogProcess)
    {
        qDebug()<<"pLogProcess log";
       pLogProcess->close();
    }

    delete pLogProcess;

    pLogProcess = 0;


//    log_adb.SendCmd("adb kill-server",1);
//    log_adb.SendCmd("adb start-server",1);



////    pLogProcess->close();
////    qDebug()<<"stop111 log222";

////    pLogProcess->kill();

////         qDebug()<<"stop111 log333";
////    pLogProcess->terminate();


//    qDebug()<<"stop111 log4444";


//   // LogProcess.kill();

//   // LogProcess.terminate();
}
