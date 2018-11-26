#include "remotecontrl.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "adb_find.h"





MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("《基于adb的Android可视化调试工具》");
    this->setWindowIcon(QIcon(":/image/log2.ico"));

    mfind =  adb_find();      //发送命令的类
    FrameRate = 0;            //帧率
    playerflag = false;       //播放与暂停标志

    kernelfastboot = false;   //控制同时上传kernel和fastboot
    PrintANDSendCMD = true;   // 会卡住的命令和播放视频共用一个定时器

    packageAndChinese = false;//应用程序为中文或英文标志
    SingleShoot = true;       //截屏专用

    Netsatus = false;          //网络连接状态检测

    MaxHeight =500;          //播放按比例显示，最大像素高度




    //与键盘关联
    ui->up_bnt->setShortcut(Qt::Key_W);//向上
    ui->down_bnt->setShortcut(Qt::Key_S);//向下
    ui->right_bnt->setShortcut(Qt::Key_A);//向左
    ui->left_bnt->setShortcut(Qt::Key_D);//向右
    ui->ok_bnt->setShortcut(Qt::Key_E);//确定
    ui->back_bnt->setShortcut(Qt::Key_Q);//返回
    ui->player->setShortcut(Qt::Key_P);//空格

    CurentDirPath = "/";               //当前目录

    downloadFileStatus=0;              //下载状态

    SendCMDCount = 0;                  //发送命令 定时器计数

    CMD_TYPE = CMD_ADB_CONNECT;        //发送命令状态




    LocalMyIp = GetLocalIP();             //获取本机IP

    ADBPath =  mfind.GetAPPPath();     //获取ADB所在路径

    ADBPath =  ADBPath + "adb.exe reboot"; //重启命令

    rc = new RemoteContrl();           //遥控器界面类初始化

    thread = new QThread();
    thread1 = new QThread();
    thread2 = new QThread();

 // thread3 = new QThread();

    thread4 = new QThread();
    thread5 = new QThread();


    mshell = new shell();              //Shell线程
    mshell->moveToThread(thread5);
    thread5->start();


    mListenThread = new listenthread();    //UDP监听服务
//  mListenThread->moveToThread(thread3);
//  thread3->start();


    mSendTread = new sendthread();       //发送命令线程，用于延时较大的命令
    mSendTread->moveToThread(thread2);
    thread2->start();

    mKmsgThread = new kmsgthread();     //获取kmsg线程
    mKmsgThread->moveToThread(thread4);
    thread4->start();


    //线程thread1：LogCat;
    mLogThread = new logthread();        //LogThread线程
    mLogThread->moveToThread(thread1);
    thread1->start();


    //线程thread：播放
    PlayerThread = new MyThread();       //播放线程
    PlayerThread->moveToThread(thread);
    thread->start();



    //开启一个定时器 用于动态播放
    Timer_Thread_Player =   new QThread(this);
    PlayerTimer =  new QTimer();

    PlayerTimer->moveToThread(Timer_Thread_Player);

    connect(Timer_Thread_Player, SIGNAL(started()), PlayerTimer, SLOT(start()));
    connect(PlayerTimer, &QTimer::timeout, this, &MainWindow::PlayertimeOutNetSlot);
    connect(Timer_Thread_Player, SIGNAL(finished()), PlayerTimer, SLOT(stop()));
    PlayerTimer->setInterval(1000);  //1秒刷新一次帧率




    //开启一个定时器 查看网络状态 ，为定时器单独开辟一个线程
    Timer_Thread_Net = new QThread(this);
    ListenTimer = new QTimer();
    ListenTimer->moveToThread(Timer_Thread_Net);

    connect(Timer_Thread_Net, SIGNAL(started()), ListenTimer, SLOT(start()));
    connect(ListenTimer, &QTimer::timeout, this, &MainWindow::timeOutNetSlot);
    connect(Timer_Thread_Net, SIGNAL(finished()), ListenTimer, SLOT(stop()));

    ListenTimer->setInterval(1000);  //1秒查看一次




    //开启一个定时器  发送耗时命令等待
    Timer_Thread = new QThread(this);
    m_pTimer = new QTimer();
    m_pTimer->moveToThread(Timer_Thread);
    m_pTimer->setInterval(500);

    connect(Timer_Thread, SIGNAL(started()), m_pTimer, SLOT(start()));
    connect(m_pTimer, &QTimer::timeout, this, &MainWindow::timeOutSlot);
    connect(Timer_Thread, SIGNAL(finished()), m_pTimer, SLOT(stop()));





    RemoteIp = ui->iptext->text();               //获取文本框盒子Ip
    mListenThread->localIp  = GetLocalIP();      //监听服务获取Ip

//  ui->BoxDirANDFile->setReadOnly(true);
//  ui->JpegPath->setReadOnly(true);



    //Logthread信号与槽

    connect(this,SIGNAL(StartPrint(QString,QString,QString)),mLogThread,SLOT(PrintLogCat(QString,QString,QString)));  //开启Logcat打印
    connect(mLogThread,SIGNAL(GetPidSingal()),this,SLOT(GetLogPiD()));   //logthread获取本机logcat pid
    connect(this,SIGNAL(StopLog()),mLogThread,SLOT(StopLogSlot()));      //logthread停止信号与槽绑定


    //列表信号与槽
    connect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(singleClickSlot(QListWidgetItem*))); //应用列表单击
    connect(ui->list_Boxfile,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(DoubleClickSlot(QListWidgetItem*))); //文件列表双击
    connect(ui->list_Boxfile,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(BoxFilesingleClickSlot(QListWidgetItem*)));//文件列表单击
    connect(this,SIGNAL(StartKmsg(QString,QString)),mKmsgThread,SLOT(PrintKmsg(QString,QString))); //开启kmsg打印


    connect(ui->pushButton,SIGNAL(pressed()),mshell,SLOT(Sysshell()));   //开启Shell


    //动态截图信号与槽
    connect(this,SIGNAL(StartPlayer(bool)),PlayerThread,SLOT(Player(bool)));  //开始动态截图
    connect(PlayerThread,SIGNAL(Start_Screenshot()),this,SLOT(UpdateShootScreen()));  //动态截图线程发挥图片给UI线程更新


    connect(this,SIGNAL(StartSendCMD(QString , int)),mSendTread,SLOT(SendCmd(QString,int)));  //发送耗时命令到线程执行

   // 设置页面的编辑   //连接IP才能使用其他功能
//    for(int i=1; i<7; i++)
//    {
//        ui->tabWidget->setTabEnabled(i,false);
//    }

    IpPort();

}



MainWindow::~MainWindow()
{
   delete ui;
   delete rc ;
   delete mListenThread;
   delete mSendTread ;
   delete mLogThread;

   delete PlayerThread;

}

//等待耗时命令

void MainWindow::timeOutSlot()
{
        int time;

        QString ret = mSendTread->ret;
        SendCMDCount++;
        time = 0.5*SendCMDCount;
        qDebug()<<"fsfs: "+QString::number(time,10) +"s";


        switch (CMD_TYPE) {
            case CMD_ADB_CONNECT:
                if(SendCMDCount>10) // 8s
                {
                    SendCMDCount = 0;


                    Timer_Thread->exit();

                    qDebug()<<"cmd long time  ";

                    ui->connect_display->setText("连接超时，正在重启ADB,请重新连接");


                   //WinExec("taskkill -f -im \"adb.exe\"",SW_HIDE);

                    mfind.SendCmd("adb kill-server",1);
                    mfind.SendCmd("adb start-server",1);

                    Timer_Thread->exit();

                    ui->connect->setEnabled(true);
                    ui->pushButton_2->setEnabled(true);

                    ui->LinkStatus->setText("连接已断开，请重新连接");


                }
                else
                {
                    if(time == 0)
                    ui->connect_display->setText("连接中。。。,耗时0.5s");
                    else
                    ui->connect_display->setText("连接中。。。,耗时"+QString::number(time,'f',1)+"s");
                }


                if(!ret.isEmpty())
                {

                    SendCMDCount =0;
                    mSendTread->ret.clear();
                    qDebug()<<ret;

                     ui->connect->setEnabled(true);
                     ui->pushButton_2->setEnabled(true);
                    if(ret.indexOf("nected")>0||ret.indexOf("start")>0)
                    {
                         //qDebug()<<"connect success";
                         Timer_Thread->exit();

                         qDebug()<<"connect sucess";

                         Netsatus = false;

                         ui->connect_display->setText("connect sucess!");

                         ui->LinkStatus->setText("已连接"+RemoteIp);
                         ui->connect->setEnabled(true);



                         Timer_Thread_Net->start();



                        for(int i = 1; i<7; i++)
                        {
                            ui->tabWidget->setTabEnabled(i,true);
                        }
                    }
                    else
                    {
                         Timer_Thread->exit();
                         qDebug()<<"connect failed";
                         ui->connect_display->setText("连接失败，正在重启ADB");
                         ui->connect->setEnabled(true);
                         ui->pushButton_2->setEnabled(true);


                         mfind.SendCmd("adb kill-server",1);
                         mfind.SendCmd("adb start-server",1);

                        // WinExec("taskkill -f -im \"adb.exe\"",SW_HIDE);

                         ui->LinkStatus->setText("连接已断开，请重新连接");

                         ui->connect_display->setText("请尝试重新连接");
                  //     on_connect_clicked();


                    }


                }
                break;
             case CMD_ADB_UPLOAD_FILE:


                if((uploadFileStatus==0))
                {
                        if(time == 0)
                        ui->file_state->setText("文件文件中。。。,耗时0.5s");
                        else
                        ui->file_state->setText("上传文件中。。。,耗时"+QString::number(time,'f',1)+"s");

                        if(ret.indexOf("ret")>0)
                        {

                            SendCMDCount =0;
                            mSendTread->ret.clear();
                            qDebug()<<ret;
                            //////////////

                            if(ret.indexOf("KB/s")<0)
                            {

                                if(ret.indexOf("Read-only")>0)
                                {
                                    ui->file_state->setText("这是一个只读的文件系统");


                                }

                                ui->upload->setEnabled(true);
                                ui->download->setEnabled(true);
                                ui->upload_dir->setEnabled(true);
                                ui->download_dir->setEnabled(true);

                                 qDebug()<<"adb push file failed";

                                 ui->file_state->append("上传文件失败");

                              //  return cmd;
                            }
                            else
                            {
                                 qDebug()<<"adb push file success";
                                 ui->file_state->append("上传文件成功");

                                 ui->upload->setEnabled(true);
                                 ui->download->setEnabled(true);
                                 ui->upload_dir->setEnabled(true);
                                 ui->download_dir->setEnabled(true);

                                 ui->list_Boxfile->clear();
                                 UpdateDir(CurentDirPath);


                            }

                            /////////////
                            Timer_Thread->exit();
                        }
                }
                else  if((uploadFileStatus==1))
                {
                        if(time == 0)
                        ui->add_state->setText("上传文件中。。。,耗时0.5s");
                        else
                        ui->add_state->setText("上传文件中。。。,耗时"+QString::number(time,'f',1)+"s");

                        if(ret.indexOf("ret")>0)
                        {


                            ui->fresh->setEnabled(true);


                            SendCMDCount =0;
                             mSendTread->ret.clear();
                            qDebug()<<ret;
                            //////////////

                            if(ret.indexOf("KB/s")<0)
                            {
                                 qDebug()<<"adb push file failed";

                                 ui->add_state->append("上传文件失败");
                                  Timer_Thread->exit();

                              //  return cmd;
                            }
                            else
                            {
                                 qDebug()<<"adb push file success";
                                 ui->add_state->append("上传文件成功");


                                 //第2条命令：adb shell "cat /data/fastboot.bin > /dev/block/platform/soc/by-name/fastboot"
                                 QString  cmd = "adb shell \"cat "+FastbootFileName+" > /dev/block/platform/"+CPU_SOC+"/by-name/fastboot\"";
                                 qDebug()<<"第2条命令："+cmd;
                                 ui->add_state->append("正在更新fastboot，重启后请重新连接");

                                 mfind.SendCmd(cmd,0);

                                 QByteArray temp = ADBPath.toLatin1(); // must
                                 char *  reboot = temp.data();

                                   //重启
                                    WinExec(reboot,SW_HIDE);
                                    ui->LinkStatus->setText("重启后连接已断开，请重新连接");


                                     Timer_Thread->exit();
                                     QMessageBox::information(this,"提示","重启后请重新连接");


                                    ui->label_pp->clear();



                            }

                            /////////////

                        }
                }
                else  if((uploadFileStatus==2))
                {
                        if(time == 0)
                        ui->add_state->setText("上传文件中。。。,耗时0.5s");
                        else
                        ui->add_state->setText("上传文件中。。。,耗时"+QString::number(time,'f',1)+"s");

                     //   qDebug()<<"fsdfds1111111";
                       if(ret.indexOf("ret")>0)
                        {

                            ui->fresh->setEnabled(true);

                            SendCMDCount =0;
                            mSendTread->ret.clear();
                            qDebug()<<ret;
                            //////////////

                            if(ret.indexOf("KB/s")<0)
                            {
                                 qDebug()<<"adb push file failed";

                                 ui->add_state->append("上传文件失败");
                                 Timer_Thread->exit();

                              //  return cmd;
                            }
                            else
                            {
                                 qDebug()<<"adb push file success";
                                 ui->add_state->append("上传文件成功");


                                 //第2条命令：adb shell "cat kernel.img > /dev/block/platform/soc/by-name/kernel"
                                  QString  cmd = "adb shell \"cat  "+KernelFileName+" > /dev/block/platform/"+CPU_SOC+"/by-name/kernel\"";
                                  qDebug()<<"第2条命令："+cmd;

                                  ui->add_state->append("正在更新内核，重启后请重新连接");



                                  mfind.SendCmd(cmd,0);

                                  QByteArray temp = ADBPath.toLatin1(); // must
                                  char *  reboot = temp.data();

                                  ui->label_op->clear();


                                 //重启
                                  WinExec(reboot,SW_HIDE);

                                   Timer_Thread->exit();

                                  ui->LinkStatus->setText("重启后连接已断开，请重新连接");
                                   QMessageBox::information(this,"提示","重启后请重新连接");

                            }

                            /////////////

                        }
                }

                else  if((uploadFileStatus==3))  //
                      {
                              if(time == 0)
                              ui->add_state->setText("一键更新中。。。,耗时0.5s");
                              else
                              ui->add_state->setText("一键更新中。。。,耗时"+QString::number(time,'f',1)+"s");

                              qDebug()<<"fsdfds1111111";
                             if(ret.indexOf("ret")>0)
                              {



//                                  if(kernelfastboot)
//                                  {
//                                      SendCMDCount =0;
//                                  }

                                  mSendTread->ret.clear();
                                  qDebug()<<ret;
                                  //////////////

                                  if(ret.indexOf("KB/s")<0)
                                  {

                                       qDebug()<<"adb push file failed";
                                        SendCMDCount =0;

                                       ui->add_state->append("上传fastboot失败");
                                       Timer_Thread->exit();

                                    //  return cmd;
                                  }
                                  else
                                  {

                                      if(!kernelfastboot)
                                      {
                                        qDebug()<<"adb push file success";
                                        ui->add_state->append("上传fastboot成功");

                                      //第2条命令：adb shell "cat /data/fastboot.bin > /dev/block/platform/soc/by-name/fastboot"

                                        QString  cmd = "adb shell \"cat "+FastbootFileName+" > /dev/block/platform/"+CPU_SOC+"/by-name/fastboot\"";


                                        qDebug()<<"第2条命令："+cmd;

                                        mfind.SendCmd(cmd,0);

                                        kernelfastboot = true;



                                        cmd = "adb push " +ui->label_op->text()+ " /data/";



                                        uploadFileStatus = 3;


                                        PrintANDSendCMD = true;

                                        CMD_TYPE =CMD_ADB_UPLOAD_FILE;

                                        emit StartSendCMD(cmd , 0);



                                      }
                                      else
                                     {

                                         ui->fresh->setEnabled(true);

                                         QString  cmd = "adb shell \"cat  "+KernelFileName+" > /dev/block/platform/"+CPU_SOC+"/by-name/kernel\"";
                                            qDebug()<<"第2条命令："+cmd;

                                         ui->add_state->append("上传kernel成功");


                                         mfind.SendCmd(cmd,0);



                                         QByteArray temp = ADBPath.toLatin1(); // must
                                         char *  reboot = temp.data();

                                         WinExec(reboot,SW_HIDE);

                                         ui->add_state->append("正在重启，请重新连接");
                                         ui->LinkStatus->setText("重启后连接已断开，请重新连接");


                                          //重启
                                         ui->label_op->clear();
                                         ui->label_pp->clear();
                                         SendCMDCount =0;

                                         Timer_Thread->exit();

                                         QMessageBox::information(this,"提示","重启后请重新连接");


                                     }


                                  }


                                                  /////////////

                       }
                break;

        case CMD_ADB_DOWNLOAD_FILE:


            if((downloadFileStatus==0))
            {

                        if(time == 0)
                        ui->file_state->setText("下载文件中。。。,耗时0.5s");
                        else
                        ui->file_state->setText("下载文件中。。。,耗时"+QString::number(time,'f',1)+"s");



                       if(ret.indexOf("ret")>0)
                       {

                           SendCMDCount =0;
                            mSendTread->ret.clear();
                           qDebug()<<ret;
                           //////////////


                           ui->upload->setEnabled(true);
                           ui->download->setEnabled(true);
                           ui->upload_dir->setEnabled(true);
                           ui->download_dir->setEnabled(true);

                           if(ret.indexOf("KB/s")<0)
                           {

                                qDebug()<<"adb push file failed";
                                ui->file_state->append("下载文件失败");

                             //  return cmd;
                           }
                           else
                           {
                                qDebug()<<"adb push file success";
                                 ui->file_state->append("下载文件成功");
                           }


                           /////////////
                           Timer_Thread->exit();
                       }
            }
            else if((downloadFileStatus==1))
            {

                if(time == 0)
                ui->add_state->setText("导出fastboot中。。。,耗时0.5s");
                else
                ui->add_state->setText("导出fastboot中。。。,耗时"+QString::number(time,'f',1)+"s");



             if(ret.indexOf("ret")>0)
               {

                   SendCMDCount =0;
                   mSendTread->ret.clear();
                   qDebug()<<ret;
                   //////////////

                   if(ret.indexOf("KB/s")<0)
                   {

                        qDebug()<<"adb push file failed";
                        ui->add_state->append("导出fastboot失败");

                     //  return cmd;
                   }
                   else
                   {
                        qDebug()<<"adb push file success";
                         ui->add_state->append("导出fastboot成功");
                   }


                   /////////////
                   Timer_Thread->exit();
               }

            }


            else if((downloadFileStatus==2))
            {

                if(time == 0)
                ui->add_state->setText("导出内核中。。。,耗时0.5s");
                else
                ui->add_state->setText("导出内核中。。。,耗时"+QString::number(time,'f',1)+"s");



              if(ret.indexOf("ret")>0)
               {

                   SendCMDCount =0;
                   mSendTread->ret.clear();
                   qDebug()<<ret;
                   //////////////

                   if(ret.indexOf("KB/s")<0)
                   {

                        qDebug()<<"adb push file failed";
                        ui->add_state->append("导出内核失败");

                     //  return cmd;
                   }
                   else
                   {
                        qDebug()<<"adb push file success";
                         ui->add_state->append("导出内核成功");
                   }


                   /////////////
                   Timer_Thread->exit();
               }

            }


           break;

        case CMD_ADB_UPLOAD_DIR:


                    if(time == 0)
                    ui->file_state->setText("上传目录中。。。,耗时0.5s");
                    else
                    ui->file_state->setText("上传目录中。。。,耗时"+QString::number(time,'f',1)+"s");
                   if(ret.indexOf("ret")>0)
                   {
                        SendCMDCount =0;
                        mSendTread->ret.clear();
                        qDebug()<<ret;
                        //////////////

                        ui->upload->setEnabled(true);
                        ui->download->setEnabled(true);
                        ui->upload_dir->setEnabled(true);
                        ui->download_dir->setEnabled(true);


                        if(ret.indexOf("KB/s")<0)
                        {
                             qDebug()<<"adb push dir failed";

                             ui->file_state->append("上传目录失败");

                          //  return cmd;
                        }
                        else
                        {
                             qDebug()<<"adb push dir success";
                             ui->file_state->append("上传目录成功");
                             ui->list_Boxfile->clear();
                             UpdateDir(CurentDirPath);


                        }

                        /////////////
                        Timer_Thread->exit();


                   }


           break;

        case CMD_ADB_DOWNLOAD_DIR:
            if(time == 0)
            ui->file_state->setText("下载目录中。。。,耗时0.5s");
            else
            ui->file_state->setText("下载目录中。。。,耗时"+QString::number(time,'f',1)+"s");


      //   ui->file_state->setText("下载目录中。。。,耗时"+QString::number(time,10) +"s");
          if(ret.indexOf("ret")>0)
           {
               SendCMDCount =0;
               mSendTread->ret.clear();
               qDebug()<<ret;
               //////////////



               ui->upload->setEnabled(true);
               ui->download->setEnabled(true);
               ui->upload_dir->setEnabled(true);
               ui->download_dir->setEnabled(true);


               if(ret.indexOf("KB/s")<0)
               {
                    ui->file_state->append("下载目录失败");
                    qDebug()<<"adb pull dir failed";
                 //  return cmd;
               }
               else
               {
                    ui->file_state->append("下载目录成功");
                    qDebug()<<"adb pull dir success";
               }

               /////////////
               Timer_Thread->exit();
           }

           break;

        case CMD_ADB_INSTALL:


           ui->mageage_state->setText("安装应用中。。。,耗时"+QString::number(time,'f',1)+"s");
            if(ret.indexOf("ret")>0)
           {
               SendCMDCount =0;
               mSendTread->ret.clear();
               qDebug()<<"install :"+ret;
               //////////////


               ui->Install->setEnabled(true);
               ui->AppList->setEnabled(true);
               ui->uninstall->setEnabled(true);
               ui->start->setEnabled(true);
               ui->stop->setEnabled(true);

               if(ret.lastIndexOf("Success")>0)
               {
                   qDebug()<<"adb Install success";
                   ui->mageage_state->append("安装应用成功");
                   Timer_Thread->exit();
                   on_AppList_clicked(); //更新列表
                 //  ui->Install->setEnabled(true);

               }
               else
               {
                   qDebug()<<"adb Install failed";
                    ui->mageage_state->append("安装应用失败");
                    Timer_Thread->exit();
                 //   ui->Install->setEnabled(true);

               }

           }

           break;

        case CMD_ADB_UPLOAD_UPDATE:
           ui->add_state->setText("推送升级中。。。,耗时"+QString::number(time,'f',1)+"s");

           if(ret.indexOf("ret")>0)
           {
               SendCMDCount =0;
               mSendTread->ret.clear();
               qDebug()<<ret;
               //////////////

               if(ret.lastIndexOf("KB/s")>0)
               {



                   QString   cmd1 = "adb shell \"echo boot-recovery > /dev/block/platform/"+CPU_SOC+"/by-name/misc\"";
                   qDebug()<<"第3条命令: "+cmd1;
                   mfind.SendCmd(cmd1,1);

                   cmd1 = "adb shell \"echo --update_package=/cache/recovery/"+ZipName+" > /cache/recovery/command\"";
                   qDebug()<<"第4条命令: "+cmd1;
                   ret = mfind.SendCmd(cmd1,1);

                   time++;
                   ui->add_state->setText("推送升级中。。。,耗时"+QString::number(time,'f',1)+"s");


                   ui->RebootpushButton->setEnabled(true);
                   ui->Recovery2_pushButton->setEnabled(true);
                   ui->Recovery3_pushButton->setEnabled(true);







                   for(int i=0; i<6; i++)
                   {
                       ui->tabWidget->setTabEnabled(i,true);
                   }


                   QByteArray temp = ADBPath.toLatin1(); // must
                   char *  reboot = temp.data();


                   WinExec(reboot,SW_HIDE);

                   ui->LinkStatus->setText("重启后连接已断开，请重新连接");

                    Timer_Thread->exit();

                   QMessageBox::information(this,"提示","重启后请重新连接");

                   qDebug()<<"update success";

                   ui->add_state->append("正在重启升级。。。");
                   ui->connect_display->setText("连接已断开");

//                 ui->add_state->append("升级成功");

               }
               else
               {
                   qDebug()<<"update failed";
                   ui->add_state->append("升级失败");
                   Timer_Thread->exit();




               }

           }

           break;

        case APP_IPADRESS:

            if(SendCMDCount>10) // 8s
            {
                SendCMDCount = 0;
                Timer_Thread->exit();
                ui->connect_display->setText("获取超时");

                  ui->App_GetIP->setEnabled(true);
               // qDebug()<<"dsssfd3";
               // thread3->exit();

            }
            else
            {

               if(time == 0)
               ui->connect_display->setText("获取IP中。。。,耗时0.5s");
               else
               ui->connect_display->setText("获取IP中。。。,耗时"+QString::number(time,'f',1)+"s");
            }

           if(!mListenThread->BroadcatIpList.isEmpty())
           {

               if(SendCMDCount>1)
               {
                   Timer_Thread->exit();
                   SendCMDCount =0;


                   ui->comboBox_IP->clear();

                   ui->connect_display->setText("获取IP成功");



                   for(int i=0; i<mListenThread->BroadcatIpList.size();i++)
                   {

                       ui->comboBox_IP->addItem(mListenThread->BroadcatIpList.at(i));
                     //qDebug()<<mListenThread->BroadcatIpList.at(i);
                   }
                       ui->App_GetIP->setEnabled(true);


                   mListenThread->BroadcatIpList.clear();

               }
           }

           break;


        case APP_GPU:

            if(SendCMDCount>10) // 8s
            {
                SendCMDCount = 0;
                Timer_Thread->exit();
                qDebug()<<"获取GPU超时";

            }
           if(!mListenThread->RetStr.isEmpty())
           {
               Timer_Thread->exit();
               SendCMDCount =0;

               ui->Box_Info->clear();
               ui->Box_Info->setText(mListenThread->RetStr);
               mListenThread->RetStr.clear();

           }

           break;


           default:
            break;
        }



    }





    //  qDebug()<<"timeOutSlot: "+QString::number(FrameRate,10);
}

void MainWindow::timeOutNetSlot()
{
    QString ip;

    ip = GetLocalIP();
    if(ip.compare(LocalMyIp)!=0)
    {

        if(!Netsatus)
        {

            ui->LinkStatus->setText(" 连接已断开");
            Timer_Thread_Net->exit();
            QMessageBox::information(this,"warning", "连接已经断开，请检查网络状态");
            Netsatus  =true;
        }


    }


    // qDebug()<<"1111:"+ip;
}

void MainWindow::PlayertimeOutNetSlot()
{

    FrameRate = PlayerThread->FrameCount;

    qDebug()<<"num "+QString::number(FrameRate,10);

    ui->frame_rate->setText("帧率："+QString::number(FrameRate,10));

    PlayerThread->FrameCount = 0;

}

void MainWindow::singleClickSlot(QListWidgetItem *item)
{


    if(packageAndChinese)
    {

        package =item->text();
        int index = package.indexOf(":");
        package = package.right(package.length()-index-1);//解析包名
    }
    else
    {
        QString temp = item->text();
        int index =  temp.indexOf(".");
        temp = temp.left(index);
        int packageIndex = temp.toInt();
        package = PackageList.at(packageIndex-1);

        index = package.indexOf(":");
        package = package.right(package.length()-index-1).trimmed();//解析包名


        //QMessageBox::information(this,"information","包名:"+package );


    }
  //  QMessageBox::information(this,"information","single click:"+package );
}

void MainWindow::BoxFilesingleClickSlot(QListWidgetItem *item)
{
   QString Chosen,temp;
   Chosen = item->text();

   temp = CurentDirPath + Chosen;
   ui->BoxDirANDFile->setText(temp);


}

void MainWindow::DoubleClickSlot(QListWidgetItem *item)
{
    QString cmd,ret;
    QString CurChosedir;
    CurChosedir =item->text();
    qDebug()<<"ada";
    if(CurChosedir.right(1).compare("/")!=0)
    {
        ChosenFile = CurChosedir;
        QMessageBox::information(this,"information","it is not a dir ");
    }
    else
    {
        ChosenDir = CurChosedir;

        CurentDirPath += CurChosedir;

        ui->list_Boxfile->clear();
        UpdateDir(CurentDirPath);








    }




}

// flag ==1  连接  flag == 0 断开连接

void MainWindow::on_connect_clicked()
{

    QString cmd;


    if(ui->comboBox_IP->currentText().isEmpty())
    {
        QMessageBox::information(this,"提示","IP地址框为空");
        return;
       //
    }



    ui->connect->setEnabled(false);
    ui->pushButton_2->setEnabled(false);

    RemoteIp = ui->comboBox_IP->currentText();

    LocalMyIp = GetLocalIP();
    cmd = "adb disconnect ";
    mfind.SendCmd(cmd,1);

    cmd = "adb connect "+RemoteIp+":"+ui->port->text();

    QTextStream txtOutput(defautarg);

    defautarg->close();
    defautarg->open(QIODevice::Truncate);
    defautarg->close();

    defautarg->open(QIODevice ::ReadWrite);

    txtOutput<<RemoteIp+":"<<ui->port->text()<<endl;


    PrintANDSendCMD = true;
    m_pTimer->setInterval(500);
    CMD_TYPE =CMD_ADB_CONNECT;


    ui->connect_display->setText("Waiting ...");
    emit StartSendCMD(cmd , 1);
    Timer_Thread->start();

}

void MainWindow::on_disconnect_clicked()
{
    QString cmd;
  //  QString ip = ui->iptext->text();

//    cmd = "adb disconnect ";
//    mfind.SendCmd(cmd,1);

    qDebug()<<"disconnect sucess";





    ui->connect->setEnabled(true);
    ui->pushButton_2->setEnabled(true);

    SendCMDCount =0;
    Timer_Thread->exit();

//  WinExec("taskkill -f -im \"adb.exe\"",SW_HIDE);
//  sleep(500);

//    ui->LinkStatus->setText(" 连接已断开");
//    for(int i=1; i<7; i++)
//    {
//        ui->tabWidget->setTabEnabled(i,false);
//    }

    ui->connect_display->setText("disconnect sucess");
}

void MainWindow::Get_Box_Info()
{
    QString cmd,ret;

    //系统版本
    cmd = "adb shell getprop ro.build.version.release";
    ret = mfind.SendCmd(cmd,1);

    ui->Box_Info->clear();
    ui->Box_Info->setText("系统版本："+ret);

}


//芯片型号
void MainWindow::on_curi_clicked()
{
    QString cmd,ret;

    //系统版本
    cmd = "adb shell getprop ro.product.chiptype";
    ret = mfind.SendCmd(cmd,1);

    ui->Box_Info->clear();
    ui->Box_Info->setText("芯片型号："+ret);
}

//DDR信息
void MainWindow::on_ddr_clicked()
{
    QString cmd,ret;

    //系统版本
    cmd = "adb shell cat proc/meminfo";
    ret = mfind.SendCmd(cmd,1);

    ui->Box_Info->clear();
    ui->Box_Info->setText("DDR信息："+ret);
}

//机型
void MainWindow::on_mache_clicked()
{
    QString cmd,ret;

    //系统版本
    cmd = "adb shell getprop ro.product.machine.model";
    ret = mfind.SendCmd(cmd,1);

    ui->Box_Info->clear();
    ui->Box_Info->setText("机型："+ret);
}

void MainWindow::on_flash_clicked()
{
    QString cmd,ret;

    //系统版本
    cmd = "adb shell getprop ro.product.flash.info";
    ret = mfind.SendCmd(cmd,1);

    ui->Box_Info->clear();
    ui->Box_Info->setText("FLASH信息："+ret);


}

void MainWindow::on_cpu_clicked()
{
    QString cmd,ret;

    //系统版本
    cmd = "adb shell cat /proc/cpuinfo";
    ret = mfind.SendCmd(cmd,1);

    ui->Box_Info->clear();
    ui->Box_Info->setText("CPU信息："+ret);
}

//GPU
void MainWindow::on_GPU_clicked()
{

    // qDebug()<<"ip:::::";
    qDebug()<<"ip:"+RemoteIp;


    mSendTread->SendApp(RemoteIp,"GPU");
    sleep(100);

    ui->Box_Info->setText( mListenThread->RetStr);
    mListenThread->RetStr.clear();

}

void MainWindow::on_num_clicked()
{
    QString cmd,ret;

    //系统版本
    cmd = "adb shell getprop ro.servialno";
    ret = mfind.SendCmd(cmd,1);

    ui->Box_Info->clear();
    ui->Box_Info->setText("序列号："+ret);
}

void MainWindow::on_mac_clicked()
{
    QString cmd,ret;

    //系统版本
    cmd = "adb shell cat /sys/class/net/eth0/address";
    ret = mfind.SendCmd(cmd,1);

    ui->Box_Info->clear();
    ui->Box_Info->setText("MAC地址："+ret);

}

void MainWindow::on_fireware_clicked()
{
    QString cmd,ret;

    //系统版本
    cmd = "adb shell getprop ro.build.product";
    ret = mfind.SendCmd(cmd,1);

    ui->Box_Info->clear();
    ui->Box_Info->setText("固件版本："+ret);
}

void MainWindow::on_kernel_clicked()
{
    QString cmd,ret;

    //系统版本
    cmd = "adb shell cat /proc/version";
    ret = mfind.SendCmd(cmd,1);

    ui->Box_Info->clear();
    ui->Box_Info->setText("内核版本："+ret);
}

void MainWindow::on_SysVer_clicked()
{
    QString cmd,ret;
    //系统版本
    cmd = "adb shell getprop ro.build.version.release";
    ret = mfind.SendCmd(cmd,1);

    ui->Box_Info->clear();
    ui->Box_Info->setText("系统版本："+ret);
}
void MainWindow::closeEvent(QCloseEvent *e)
{

 // on_disconnect_clicked();

//  mfind.SendCmd("adb disconnect ",1);




//  mListenThread->stop = true;
//  sleep(100);
//  thread3->exit();

}


//打印log
void MainWindow::on_print_clicked()
{
       PrintANDSendCMD= true;
       QString cmd;
       QString pid;
       QString filename,name1,fileroute;  //保存log文件的路径,文件名

        mLogThread->status = false;

       qDebug()<<"name1:...."+name1;

       pid = ui->PID->text();
       cmd = ui->cmd_display->toPlainText();

       qDebug()<<"传入线程的log命令:"<<cmd;

       JPegPath  =  ui->JpegPath->text();


       datetime = QDateTime::currentDateTime();//
       QString defautfilename = JPegPath+"/"+ datetime.toString("yyyy-MM-dd-hh-mm-ss-zzz")+"logcat.txt";


       filename = QFileDialog::getSaveFileName(this, tr("Save File"), "./"+defautfilename, tr("txt (*.txt)"));


       qDebug()<<"传入线程filename:"<<filename;
       if(filename.isEmpty())
           return;

//       int index =  filename.lastIndexOf("/");

//       name1 = filename.right(filename.length() - index-1);

//       qDebug()<<



//       fileroute = filename.left(index);

//       if(fileroute.right(1).compare(":")==0)
//       {
//           fileroute = fileroute+"/";
//       }


//       qDebug()<<"filename :"<<name1+"ssd: "+fileroute;




       emit StartPrint(pid, cmd, filename);


}

//上传文件
void MainWindow::on_upload_clicked()
{
    QString filename;
    QString cmd,ret,uproad;
    uproad = ui->BoxDirANDFile->text();



    if(uproad.right(1).compare("/")!=0)
    {
          QMessageBox::information(this,"warning","please chose a dir");
          return ;
    }

    ui->upload->setEnabled(false);
    ui->download->setEnabled(false);
    ui->upload_dir->setEnabled(false);
    ui->download_dir->setEnabled(false);

    if(uploadDir.isEmpty())
    {
        uploadDir = ".";
    }
    filename = QFileDialog::getOpenFileName(this,tr("选择文件"),uploadDir);
    if(filename.isEmpty())
    {

        ui->upload->setEnabled(true);
        ui->download->setEnabled(true);
        ui->upload_dir->setEnabled(true);
        ui->download_dir->setEnabled(true);

        return;
    }

    int index = filename.lastIndexOf("/");
    uploadDir = filename.left(index);

    cmd = "adb push "+filename+" "+uproad;
    qDebug()<<"cmd:  "+cmd;


     PrintANDSendCMD = true;
     uploadFileStatus = 0;
     CMD_TYPE =CMD_ADB_UPLOAD_FILE;
    emit StartSendCMD(cmd , 0);
     m_pTimer->setInterval(500);
    Timer_Thread->start();

}
//下载文件
void MainWindow::on_download_clicked()
{
    downloadFileStatus =0;
    QString filename,dir;
    QString cmd,ret,downroad;


    downroad = ui->BoxDirANDFile->text();
    if(downroad.right(1).compare("/")==0)
    {
          QMessageBox::information(this,"warning","please chose a file");
          return ;
    }


    if(downroadDir.isEmpty())
    {
        downroadDir = ".";
    }

    filename =QFileDialog::getExistingDirectory(this,tr("Open Directory"), downroadDir,
                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(filename.isEmpty())
        return;



    int index = filename.lastIndexOf("/");
    downroadDir = filename.left(index);



    ui->upload->setEnabled(false);
    ui->download->setEnabled(false);
    ui->upload_dir->setEnabled(false);
    ui->download_dir->setEnabled(false);

    cmd = "adb pull "+downroad+" "+filename;
    qDebug()<<"cmd download:  "+cmd;


    PrintANDSendCMD = true;
    CMD_TYPE =CMD_ADB_DOWNLOAD_FILE;
    emit StartSendCMD(cmd , 0);

     m_pTimer->setInterval(500);
    Timer_Thread->start();

}

//上传目录
void MainWindow::on_upload_dir_clicked()
{
    QString filename;
    QString cmd,ret,uproad;

    uproad = ui->BoxDirANDFile->text();
    if(uproad.right(1).compare("/")!=0)
    {
          QMessageBox::information(this,"warning","please chose a dir");
          return ;
    }

    if(uploadDir.isEmpty())
    {
        uploadDir = ".";
    }


    filename =QFileDialog::getExistingDirectory(this,tr("选择目录"), uploadDir,
                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(filename.isEmpty())
        return;

    qDebug()<<"111 :"+filename;

    int index = filename.lastIndexOf("/");
    uploadDir = filename.left(index);


    ui->upload->setEnabled(false);
    ui->download->setEnabled(false);
    ui->upload_dir->setEnabled(false);
    ui->download_dir->setEnabled(false);



    cmd = "adb push "+filename+" "+uproad;
    qDebug()<<"cmd:  "+cmd;


     PrintANDSendCMD = true;

    CMD_TYPE =CMD_ADB_UPLOAD_DIR;
    emit StartSendCMD(cmd , 0);
     m_pTimer->setInterval(500);
    Timer_Thread->start();
}

//下载目录
void MainWindow::on_download_dir_clicked()
{
    QString filename;
    QString cmd,ret,downroad;


    downroad = ui->BoxDirANDFile->text();
    if(downroad.right(1).compare("/")!=0)
    {
          QMessageBox::information(this,"warning","please chose a dir");
          return ;
    }

    if(downroadDir.isEmpty())
    {
        downroadDir = ".";
    }

    filename =QFileDialog::getExistingDirectory(this,tr("Open Directory"), downroadDir,
                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);


    int index = filename.lastIndexOf("/");
    downroadDir = filename.left(index);


    ret =  downroad.right(1);
    if(ret.compare("/")!=0)
    {
        QMessageBox::information(this,"提示",downroad+" 错误，请输入一个目录");
        return ;
    }

    if(filename.isEmpty())
        return;



    ui->upload->setEnabled(false);
    ui->download->setEnabled(false);
    ui->upload_dir->setEnabled(false);
    ui->download_dir->setEnabled(false);


    cmd = "adb pull "+downroad+" "+filename;
    qDebug()<<"cmd download:  "+cmd;


     PrintANDSendCMD = true;
    CMD_TYPE =CMD_ADB_DOWNLOAD_DIR;
    emit StartSendCMD(cmd , 0);
     m_pTimer->setInterval(500);
    Timer_Thread->start();

}

//获取应用列表
void MainWindow::on_AppList_clicked()
{
    QString cmd,ret;


    ui->AppList->setEnabled(false);
    ui->Install->setEnabled(false);
    ui->uninstall->setEnabled(false);
    ui->start->setEnabled(false);
    ui->stop->setEnabled(false);



    ui->mageage_state->setText("正在获取应用列表，请稍后");

    StringList.clear();

    ui->listWidget->clear();

    cmd = "adb shell pm list package -e";
    ret = mfind.SendCmd(cmd,1);


 //   APP_LIST
//    ret = ret.section('\n', 1, 1);

   // qDebug()<<"sss: "<<ret;


   //  qDebug()<<"ret:"<<ret;

    PackageList = ret.split("\n");

    int len = PackageList.size();
    qDebug()<<"len1 :"+QString::number(len,10);

    QString applistcmd = "GetAppName";
    QString temp,AppName;

    int count= 0;

    for (int i = 0; i < PackageList.size(); ++i)
    {
       count++;
       if(count==10)
       {
           qDebug()<<"yi hang";
           count=0;
           mSendTread->SendApp(RemoteIp,applistcmd);
           sleep(100);

          // if(mListenThread->RetStr.indexOf("Unionman")>0) break;

             AppName += "|"+mListenThread->RetStr;
             mListenThread->RetStr.clear();


          //   qDebug()<<"ret111: "+AppName;


             if(AppName.length()<10)
             {
                 qDebug()<<"111";
                 packageAndChinese = true;
                 break;
             }else
             {
                packageAndChinese = false;
             }

           applistcmd = "GetAppName";

       }

       int index = PackageList.at(i).indexOf(":");
       temp = PackageList.at(i).right(PackageList.at(i).length()-index-1);
       applistcmd = applistcmd+"|"+temp.trimmed().simplified();
    }

    mSendTread->SendApp(RemoteIp,applistcmd);
    sleep(100);
    AppName += "|"+mListenThread->RetStr;
      mListenThread->RetStr.clear();

    StringList = AppName.split("|");
    AppName.clear();
    len = StringList.size();
    qDebug()<<"len2 :"+QString::number(len,10);


   // qDebug()<<"AppName: "+AppName;


    if(StringList.size()<10)
    {
        for(int j=0; j<PackageList.size()-1;j++)
        {
            QListWidgetItem *item = new QListWidgetItem;
            item->setText(QString::number(j+1,10)+"."+PackageList.at(j));
            ui->listWidget->addItem(item);
        }

    }
    else
    {

        for(int j=1; j<StringList.size();j++)
        {
            QListWidgetItem *item = new QListWidgetItem;
            item->setText(QString::number(j,10)+"."+StringList.at(j)+"("+PackageList.at(j-1)+")");
            ui->listWidget->addItem(item);
        }

    }

    ui->AppList->setEnabled(true);
    ui->Install->setEnabled(true);
    ui->uninstall->setEnabled(true);
    ui->start->setEnabled(true);
     ui->stop->setEnabled(true);


    ui->mageage_state->setText("获取应用列表成功");



}

//安装应用
void MainWindow::on_Install_clicked()
{


    QString filename;
    QString cmd,ret;

    filename = QFileDialog::getOpenFileName(this,tr("选择文件"), ".",tr("*.apk"));
    if(filename.isEmpty())
        return;

    ui->Install->setEnabled(false);
    ui->AppList->setEnabled(false);
    ui->uninstall->setEnabled(false);
    ui->start->setEnabled(false);
    ui->stop->setEnabled(false);



    cmd = "adb install -r "+filename;
    qDebug()<<"cmd:  "+cmd;


     PrintANDSendCMD = true;
    CMD_TYPE =CMD_ADB_INSTALL;
    emit StartSendCMD(cmd , 1);
     m_pTimer->setInterval(500);
    Timer_Thread->start();
}

//卸载应用
void MainWindow::on_uninstall_clicked()
{

    QString cmd,ret;
    QStringList UninstallList;

    ui->Install->setEnabled(false);
    ui->AppList->setEnabled(false);
    ui->uninstall->setEnabled(false);
    ui->start->setEnabled(false);
    ui->stop->setEnabled(false);


//    if(ui->listWidget->size()<1)
//    {
//        QMessageBox::information(this,"提示","请获取应用列表并选中应用之后卸载应用");
//        return;

//    }


    if(package.isEmpty())
    {
        QMessageBox::information(this,"提示","请获取应用列表之后选择应用");
        return;
    }


    cmd = "adb  uninstall "+package;
    qDebug()<<"cmd:  "+cmd;

    ret = mfind.SendCmd(cmd,1);
    qDebug()<<"uninstall:"+ret;


    if(ret.indexOf("ccess")>0)
    {
        qDebug()<<"adb uninstall success";
        ui->mageage_state->append("卸载应用"+package.trimmed()+"成功");
        on_AppList_clicked(); //更新列表

    }
    else
    {
        qDebug()<<"adb uninstall failed";
        ui->mageage_state->append("卸载应用"+package.trimmed()+"失败");

    }

    ui->Install->setEnabled(true);
    ui->AppList->setEnabled(true);
    ui->uninstall->setEnabled(true);
    ui->start->setEnabled(true);
    ui->stop->setEnabled(true);


}

//启动
void MainWindow::on_start_clicked()
{
    if(package.isEmpty())
    {
        QMessageBox::information(this,"提示","请获取应用列表之后选择应用");
        return;
    }

    QString cmd ="adb shell monkey -p "+package+" 1";
    qDebug()<<"启动命令:"<<cmd;
    emit StartSendCMD(cmd , 0);
}
//停止
void MainWindow::on_stop_clicked()
{
    if(package.isEmpty())
    {
        QMessageBox::information(this,"提示","请获取应用列表之后选择应用");
        return;
    }

    QString cmd ="adb shell am force-stop "+package;
    qDebug()<<"停止命令:"<<cmd;
    emit StartSendCMD(cmd , 0);
}

//截屏
void MainWindow::on_shootscreen_clicked()
{

      JPegPath  =  ui->JpegPath->text();

    if(ui->JpegPath->text().isEmpty())
    {
       QMessageBox::information(this,"提示","请先选择保存目录");
       //return;
    }
    else
    {
         if(playerflag)
          {

              QPixmap map =  PlayerThread->map;
              datetime = QDateTime::currentDateTime();//设置字体
              QString filename = JPegPath+"/"+ datetime.toString("yyyy-MM-dd-hh-mm-ss-zzz")+".jpg";





               map.save(filename);


          }
          else
          {
             PlayerThread->Screenshot();
          }
    }

   // PlayerThread->Screenshot();

}

//播放
void MainWindow::on_player_clicked()
{
     PrintANDSendCMD = false;

    if(!playerflag)
    {

        ui->player->setText("暂停");
        PlayerTimer->setInterval(1000);
        Timer_Thread_Player->start();

        playerflag = true;
        SingleShoot = false;
        PlayerThread->playstate = false;
        emit StartPlayer(playerflag);
    }
    else
    {
        ui->player->setText("播放");
       // Timer_Thread->terminate();

        Timer_Thread_Player->exit();
        playerflag = false;

        SingleShoot = true;

        PlayerThread->playstate=true;
        FrameRate =0;
        ui->frame_rate->setText("帧率："+QString::number(FrameRate,10));
        qDebug()<<"已经停止";
    }

}

void MainWindow::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


int MainWindow::Listener()
{
        WSADATA wsaData;
        WORD sockVersion = MAKEWORD(2,2);
        if(WSAStartup(sockVersion, &wsaData) != 0)
        {
            return 0;
        }

        SOCKET serSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if(serSocket == INVALID_SOCKET)
        {
            printf("socket error !");
            return 0;
        }

        struct sockaddr_in serAddr;
        serAddr.sin_family = AF_INET;
        serAddr.sin_port = htons(12345);
        serAddr.sin_addr.S_un.S_addr = INADDR_ANY;
        if(bind(serSocket, (struct sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
        {
            printf("bind error !");
            closesocket(serSocket);
            return 0;
        }

       struct sockaddr_in remoteAddr;
        int nAddrLen = sizeof(remoteAddr);
        while (1)
        {
            char recvData[255];
            int ret = recvfrom(serSocket, recvData, 255, 0, (struct sockaddr *)&remoteAddr, &nAddrLen);
            if (ret > 0)
            {
                recvData[ret] = 0x00;
                printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));
                printf(recvData);
            }

            char * sendData = "一个来自服务端的UDP数据包\n";
            sendto(serSocket, sendData, strlen(sendData), 0, (struct sockaddr *)&remoteAddr, nAddrLen);

        }
        closesocket(serSocket);
        WSACleanup();
        return 0;
}

void MainWindow::UpdateShootScreen()
{


   // qDebug()<<"111";

     JPegPath  =  ui->JpegPath->text();


    QPixmap map =  PlayerThread->map;

    if(SingleShoot)
    {
        if(ui->JpegPath->text().isEmpty())
        {
           QMessageBox::information(this,"提示","请先选择保存目录");
           return;
        }

        datetime = QDateTime::currentDateTime();//设置字体
        QString filename = JPegPath+"/"+ datetime.toString("yyyy-MM-dd-hh-mm-ss-zzz")+".jpg";
        map.save(filename);

    }

    int Owidth=map.width(),Oheight=map.height();
    int Fwidth,Fheight;       //缩放后的图片大小
    float Mul;            //记录图片与label大小的比例，用于缩放图片

    if(Owidth /  ui->pic_display->width() > Oheight /  ui->pic_display->height())

       Mul = Owidth /  ui->pic_display->width();
    else
       Mul = Oheight /  ui->pic_display->height();


  // if(Mul==0) Mul = 1;

//    qDebug()<<"113";

//    qDebug()<<"112:"+QString::number(Owidth /ui->pic_display->width(),10);
//    qDebug()<<"112:"+QString::number(Oheight /  ui->pic_display->height(),10);

//    qDebug()<<"112:"+QString::number((int)(Owidth/Mul),10);



    Fwidth = (int)(Owidth/Mul);
    Fheight = (int) (Oheight/Mul);


    if(Fheight>MaxHeight)
    {
        Fheight = MaxHeight;
        Fwidth = Fheight* Owidth/ Fheight;

    }


    ui->pic_display->setPixmap(map.scaled(Fwidth,Fheight,Qt::KeepAspectRatio));




}

//aaaaaaaaaa
//void MainWindow::on_send_clicked()
//{

//    SendApp(MyIp,"GiveMeIp");
//}

void MainWindow::on_listen_clicked()
{
   // emit startListener1();
    emit StartListen();
}

void MainWindow::on_print_kmsg_clicked()
{
    QString name,fileroute,filename;     //kmsg文件的路径和文件名


    datetime = QDateTime::currentDateTime();//设置字体
    QString defautfilename = JPegPath+"/"+ datetime.toString("yyyy-MM-dd-hh-mm-ss-zzz")+"kmsg.txt";


    name = QFileDialog::getSaveFileName(this, tr("Save File"), "./"+defautfilename, tr("txt (*.txt)"));


    mKmsgThread->status = false;


    int index =  name.lastIndexOf("/");

      filename = name.right(name.length() - index-1);
      fileroute = name.left(index);


      if(fileroute.right(1).compare(":")==0)
      {
          fileroute = fileroute+"/";
      }



      qDebug()<<"filename :"+filename+"path: "+fileroute;


       if(fileroute.isEmpty())
           return;
       emit StartKmsg(fileroute,filename);

}

void MainWindow::on_roteCotrul_pushButton_clicked()
{
    rc->show();
}

//启动recovery 恢复出厂设置
void MainWindow::on_Recovery1_pushButton_clicked()
{
    int index = QMessageBox::information(this,"恢复出厂设置提示框","恢复出厂设置会清除用户所有数据\n是否确定","确定","取消");
    if(index == 1)
        return;
    QString cmd,ret;
    cmd = "adb shell wipe data";
    ret = mfind.SendCmd(cmd,1);
    qDebug()<<"清除所有数据: "+cmd;

    QByteArray temp = ADBPath.toLatin1(); // must
    char *  reboot = temp.data();


    if(WinExec(reboot,SW_HIDE)>31)
      ui->add_state->setText("恢复出厂设置成功。。。");

    ui->LinkStatus->setText("重启后连接已断开，请重新连接");
    ui->connect_display->setText("连接已断开");

     QMessageBox::information(this,"提示","重启后请重新连接");

    cmd = "adb reboot";

    qDebug()<<"重启: "+cmd;
}
//启动recovery u盘升级
void MainWindow::on_Recovery2_pushButton_clicked()
{
    QString cmd,ret;
    QStringList dirlist;
    bool status = false;

    int index1 = QMessageBox::information(this,"U盘消息框","是否进行U盘升级\n","确定","取消");
    if(index1 == 1)
        return;


    cmd = "adb shell ls  /mnt/";
    ret = mfind.SendCmd(cmd,1);
    dirlist = ret.split("\n");



    for(int i=0;i<dirlist.size()-1;i++)
    {

         QString temp = dirlist.at(i).trimmed().simplified();

         if(temp.compare("sda")==0)
         {
             status = true;
             break;

         }
    }

    if(!status)
    {

        QMessageBox::information(this,"提示","请先插入U盘");
        return;
    }



    cmd  ="adb shell  ls /dev/block/platform ";
    ret = mfind.SendCmd(cmd,1).trimmed().simplified();
    CPU_SOC = ret;



    cmd = "adb shell \"echo boot-recovery > /dev/block/platform/"+CPU_SOC+"/by-name/misc\"";
    qDebug()<<"U盘升级: "+cmd;
    ret = mfind.SendCmd(cmd,1);


    QByteArray temp = ADBPath.toLatin1(); // must
    char *  reboot = temp.data();

    WinExec(reboot,SW_HIDE);
    ui->LinkStatus->setText("重启后连接已断开，请重新连接");
    QMessageBox::information(this,"提示","U盘升级后请重新连接");

    ui->connect_display->setText("连接已断开");

    ui->add_state->setText("正在重启升级。。。，重启后连接已断开，请重新连接");


    cmd = "adb reboot";
    qDebug()<<"重启命令: "+cmd;

}

//推送update.zip包并启动recovery升级
void MainWindow::on_Recovery3_pushButton_clicked()
{
    QString filename;
    QString cmd,ret;
    int index1 = QMessageBox::information(this,"推送升级包消息框","是否进行推送升级\n","确定","取消");
    if(index1 == 1)
        return;


    cmd  ="adb shell  ls /dev/block/platform ";
    ret = mfind.SendCmd(cmd,1).trimmed().simplified();
    CPU_SOC = ret;




    cmd = "adb shell \"mkdir -p /cache/recovery\"";//为安装包创建文件夹
    qDebug()<<"第1条命令: "+cmd;
    ret = mfind.SendCmd(cmd,1);

    filename = QFileDialog::getOpenFileName(this,tr("选择升级包"), ".",tr("*.zip"));
    if(filename.isEmpty())
         return;


    int index = filename.lastIndexOf("/");
    ZipName = filename.right(filename.length() - index-1);




    ui->RebootpushButton->setEnabled(false);
    ui->Recovery2_pushButton->setEnabled(false);
    ui->Recovery3_pushButton->setEnabled(false);

    for(int i=0; i<6; i++)
    {
        ui->tabWidget->setTabEnabled(i,false);
    }






    cmd = "adb push "+filename +" /cache/recovery";
    qDebug()<<"第2条命令: "+cmd;

    PrintANDSendCMD = true;
    CMD_TYPE =CMD_ADB_UPLOAD_UPDATE;
    emit StartSendCMD(cmd , 0);
    m_pTimer->setInterval(500);
    Timer_Thread->start();




}
//重启
void MainWindow::on_RebootpushButton_clicked()
{
    QString cmd,ret;
    int index = QMessageBox::information(this,"重启消息框","是否确定重启\n","确定","取消");
    if(index == 1)
        return;

    QByteArray temp = ADBPath.toLatin1(); // must
    char *  reboot = temp.data();


    WinExec(reboot,SW_HIDE);

    ui->LinkStatus->setText("重启后连接已断开，请重新连接");
    QMessageBox::information(this,"提示","重启后请重新连接");

    ui->connect_display->setText("连接已断开");


    ui->add_state->setText("正在重启升级。。。，重启后连接已断开，请重新连接");

    cmd = "adb reboot";
    qDebug()<<"重启: "+cmd;
}




void MainWindow::on_pushButton_2_clicked()
{

    if(ui->iptext->text().isEmpty())
    {
        QMessageBox::information(this,"提示","请输入IP地址");
        return;
    }

    ui->pushButton_2->setEnabled(false);
    ui->connect->setEnabled(false);

    QString  cmd;
    RemoteIp = ui->iptext->text();


    LocalMyIp = GetLocalIP();
    cmd = "adb disconnect ";
    mfind.SendCmd(cmd,1);

    cmd = "adb connect "+RemoteIp+":"+ui->port->text();;
    PrintANDSendCMD = true;
    m_pTimer->setInterval(500);

    CMD_TYPE =CMD_ADB_CONNECT;

    QTextStream txtOutput(defautarg);

    defautarg->close();
    defautarg->open(QIODevice::Truncate);
    defautarg->close();

    defautarg->open(QIODevice ::ReadWrite);

    txtOutput<<RemoteIp+":"<<ui->port->text()<<endl;



    ui->connect_display->setText("Waiting ...");
    emit StartSendCMD(cmd , 1);
    Timer_Thread->start();

}

//导出fastboot
void MainWindow::on_DownLoadFastboot_clicked()
{
    QString filename,fastbootdir;
    QString cmd,ret;

    filename =QFileDialog::getExistingDirectory(this,tr("Open Directory"), "/home",
                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(filename.isEmpty())
        return;


     qDebug()<<"filename:  "+filename;


     cmd  ="adb shell  ls /dev/block/platform ";
     ret = mfind.SendCmd(cmd,1).trimmed().simplified();




    //第一条命令   adb shell ls -l /dev/block/platform/soc/by-name/fastboot
    cmd = "adb shell ls -l /dev/block/platform/"+ret+"/by-name/fastboot";

    qDebug()<<"cmd:  "+cmd;

    ret = mfind.SendCmd(cmd,1);
    int index= ret.indexOf("/");

    fastbootdir = ret.right(ret.length()-index);


    //第二条命令   adb shell ls -l /dev/block/platform/soc/by-name/fastboot
    cmd = "adb shell dd if="+fastbootdir+"  of=/sdcard/fastboot.img";
    ret = mfind.SendCmd(cmd,1);

    if(ret.indexOf("transferred")>0)
    {
        qDebug()<<"Make fastboot.img success";
    }

    //第三条命令   adb shell ls -l /dev/block/platform/soc/by-name/fastboot

    cmd = "adb pull  /sdcard/fastboot.img "+filename;

    qDebug()<<"cmd download:  "+cmd;

    PrintANDSendCMD = true;
    downloadFileStatus =1;
    CMD_TYPE =CMD_ADB_DOWNLOAD_FILE;
    emit StartSendCMD(cmd , 0);

    m_pTimer->setInterval(500);
    Timer_Thread->start();

}

//导出内核
void MainWindow::on_DownLoadKernel_clicked()
{
    QString filename,kerneldir;
    QString cmd,ret;

    filename =QFileDialog::getExistingDirectory(this,tr("Open Directory"), "/home",
                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(filename.isEmpty())
        return;




    ui->add_state->setText("正在制作内核");



    cmd  ="adb shell  ls /dev/block/platform ";
    ret = mfind.SendCmd(cmd,1).trimmed().simplified();
    CPU_SOC = ret;



     qDebug()<<"filename:  "+filename;
    //第一条命令   adb shell ls -l /dev/block/platform/soc/by-name/fastboot
    cmd = "adb shell ls -l /dev/block/platform/"+ret+"/by-name/kernel";
    qDebug()<<"cmd:  "+cmd;

    ret = mfind.SendCmd(cmd,1);
    int index= ret.indexOf("/");

    kerneldir = ret.right(ret.length()-index);


    //第二条命令   adb shell ls -l /dev/block/platform/soc/by-name/fastboot
    cmd = "adb shell dd if="+kerneldir+"  of=/sdcard/kernel.img";


    ret = mfind.SendCmd(cmd,1);

    if(ret.indexOf("transferred")>0)
    {
        ui->add_state->setText("内核制作成功");
        qDebug()<<"Make kernel.img success";
    }

    //第三条命令   adb shell ls -l /dev/block/platform/soc/by-name/fastboot

    cmd = "adb pull  /sdcard/kernel.img "+filename;



    PrintANDSendCMD = true;
    downloadFileStatus =2;
    CMD_TYPE =CMD_ADB_DOWNLOAD_FILE;
    emit StartSendCMD(cmd , 0);

    m_pTimer->setInterval(500);
    Timer_Thread->start();


}
QString MainWindow::GetLocalIP()
{

    QString ipAddr;
    QString localHost  = QHostInfo::localHostName();

    QHostInfo info = QHostInfo::fromName(localHost);
    info.addresses();//QHostInfo的address函数获取本机ip地址
    //如果存在多条ip地址ipv4和ipv6：
    foreach(QHostAddress address,info.addresses())
    {
        if(address.protocol()==QAbstractSocket::IPv4Protocol){//只取ipv4协议的地址
            ipAddr = address.toString();
        }
    }
   // qDebug()<<"IP : "<<ipAddr;
    return ipAddr;

}

void MainWindow::ScanIP()
{

     QString ret;
     LocalIP =  GetLocalIP();


     ret = mfind.SendCmd("arp -a",1);

    qDebug()<<"ret: "+ret;

}

void MainWindow::UpdateDir(QString Chosendir)
{



    QFileIconProvider icon_provider;


    QString cmd,ret,add;
    QStringList dirlist;
    int start,end;

  //  Chosendir = Chosendir.left(Chosendir.length()-1);

     cmd = "adb shell ls "+Chosendir+" -F";

     qDebug()<<"cmd: "+Chosendir;

    ret = mfind.SendCmd(cmd,1);
    dirlist = ret.split("\n");
    for(int i=0;i<dirlist.size()-1;i++)
    {
       // qDebug()<<dirlist.at(i);
        int SpaceIndex = dirlist.at(i).indexOf(" ");

      //  qDebug()<<"space ；"+QString::number(SpaceIndex,10);

        if(dirlist.at(i).indexOf("No such")>0)
        {
            continue;
           //qDebug()<<"continus";
        }

        if(SpaceIndex==1)
        {
            if(dirlist.at(i).left(1).compare("d")==0)
            {
              //  qDebug()<<"dir";
                add = "/";


            }
            else
            {
              //  qDebug()<<"dir";
                add = "";
            }
        }
        else
        {
            if(dirlist.at(i).left(2).compare("ld")==0)
            {
              //  qDebug()<<"dir";
                add = "/";
            }
            else
            {
              //  qDebug()<<"dir";
                add = "";
            }
        }


        start = dirlist.at(i).indexOf(" ");
        end = dirlist.at(i).indexOf("\n");

        QString temp  =  dirlist.at(i).mid(start+1,end-2).trimmed().simplified()+add;



        if(add.compare("/"))
        {
            QIcon icon = icon_provider.icon(QFileIconProvider::File);

            QListWidgetItem *item = new QListWidgetItem(icon,temp);
            ui->list_Boxfile->addItem(item);
        }
        else
        {
            QIcon icon = icon_provider.icon(QFileIconProvider::Folder);

            QListWidgetItem *item = new QListWidgetItem(icon,temp);
            ui->list_Boxfile->addItem(item);
        }







      // qDebug()<<"temp:"+temp;

        CurentDir<<temp;


    }

//    for(int i=0;i<CurentDir.size();i++)
//    {

//        qDebug()<<CurentDir.at(i);
//    }


}

void MainWindow::on_App_GetIP_clicked()
{
    QString ip = GetLocalIP();

    ui->App_GetIP->setEnabled(false);

    qDebug()<<"guangboip "+ip;

    int index =  ip.lastIndexOf(".");
    ip = ip.left(index+1)+"255";

    qDebug()<<"guangbo "+ip;

    mListenThread->stop = false;


    mSendTread->SendApp(ip,"GiveMeIp");


    ui->connect_display->setText("正在获取Ip...");

    PrintANDSendCMD = true;
    CMD_TYPE = APP_IPADRESS;
    m_pTimer->setInterval(500);
    Timer_Thread->start();

}

void MainWindow::on_Scan_clicked()
{
    QString ip = GetLocalIP();

 //   qDebug()<<"guangboip "+ip;

    int index =  ip.lastIndexOf(".");
    ip = ip.left(index+1)+"255";

 //   qDebug()<<"guangbo "+ip;

    mSendTread->SendApp(ip,"GiveMeIp");
}


void MainWindow::on_pushButton_clicked()
{




   // system("adb shell");
}

void MainWindow::on_ScanBoxDir_clicked()
{
    QString cmd,ret,add;
    QStringList dirlist;
    QFileIconProvider icon_provider;


    ui->list_Boxfile->clear();
    int start,end;
    cmd = "adb shell ls -F";
    ret = mfind.SendCmd(cmd,1);
    dirlist = ret.split("\n");
    CurentDirPath = "/";



    for(int i=0;i<dirlist.size()-1;i++)
    {
       // qDebug()<<dirlist.at(i);


        int SpaceIndex = dirlist.at(i).indexOf(" ");

      //  qDebug()<<"space ；"+QString::number(SpaceIndex,10);


        if(dirlist.at(i).indexOf("No such")>0)
        {
            continue;
        }

        if(SpaceIndex==1)
        {
            if(dirlist.at(i).left(1).compare("d")==0)
            {
              //  qDebug()<<"dir";
                add = "/";
            }
            else
            {
              //  qDebug()<<"dir";
                add = "";
            }
        }
        else
        {
            if(dirlist.at(i).left(2).compare("ld")==0)
            {
              //  qDebug()<<"dir";
                add = "/";
            }
            else
            {
              //  qDebug()<<"dir";
                add = "";
            }
        }





        start = dirlist.at(i).indexOf(" ");
        end = dirlist.at(i).indexOf("\n");

        QString temp  =  dirlist.at(i).mid(start+1,end-2).trimmed().simplified()+add;





        if(add.compare("/"))
        {
            QIcon icon = icon_provider.icon(QFileIconProvider::File);

            QListWidgetItem *item = new QListWidgetItem(icon,temp);
            ui->list_Boxfile->addItem(item);
        }
        else
        {
            QIcon icon = icon_provider.icon(QFileIconProvider::Folder);

            QListWidgetItem *item = new QListWidgetItem(icon,temp);
            ui->list_Boxfile->addItem(item);
        }









      // qDebug()<<"temp:"+temp;

        CurentDir<<temp;



    }

//    for(int i=0;i<CurentDir.size();i++)
//    {

//        qDebug()<<CurentDir.at(i);
//    }



  //  qDebug()<<"ret："+ret;

}

void MainWindow::on_dir_back_clicked()
{
   // QString cmmm = "11111aa/saad/dss/d/fd/a";
    if(CurentDirPath.compare("/")==0)
    {
        QMessageBox::information(this,"warning ","this is a root dir");


    }
    else
    {
       ui->list_Boxfile->clear();
       CurentDirPath = CurentDirPath.left(CurentDirPath.length()-1);

       int index =  CurentDirPath.lastIndexOf("/");
       CurentDirPath =  CurentDirPath.left(index+1);
       UpdateDir(CurentDirPath);

       ui->BoxDirANDFile->setText(CurentDirPath);
    }




//   qDebug()<<"path:"+CurentDirPath ;

}

void MainWindow::on_Create_Dir_clicked()
{
   QString dir  = ui->addBox_dir->text();
   QString cmd,ret;



   if(ui->list_Boxfile->count() < 1 )
   {
         QMessageBox::information(this,"warning ","please scan dir ");
         return;
   }





    if(dir.isEmpty())
    {
          QMessageBox::information(this,"warning ","please input a dir");
          return;
    }


 ///  qDebug()<<"CurentDir:"+CurentDirPath;

   cmd = "adb shell mkdir "+CurentDirPath+dir;

     qDebug()<<"CurentDir:"+cmd;
   ret = mfind.SendCmd(cmd,1);
   if(ret.indexOf("Read-only")>0)
   {
        QMessageBox::information(this,"warning", CurentDirPath+" is Read-only file system");
        return;
   }

   if(ret.isEmpty())
   {
        QMessageBox::information(this,"warning", CurentDirPath+"创建成功");
        ui->list_Boxfile->clear();
        UpdateDir(CurentDirPath);
        return;
   }

   if(ret.indexOf("File exists")>0)
   {
        QMessageBox::information(this,"warning", dir+"已经存在");
        return;
   }


   qDebug()<<"ret:"+ret;




}

void MainWindow::on_DeleteDirFile_clicked()
{
    QString dir  = ui->BoxDirANDFile->text();
    QString cmd,ret;



    if(ui->list_Boxfile->count() < 1 )
    {
          QMessageBox::information(this,"warning ","please scan dir ");
          return;
    }





     if(dir.isEmpty())
     {
           QMessageBox::information(this,"warning ","please slect a dir or file");
           return;
     }


  ///  qDebug()<<"CurentDir:"+CurentDirPath;

    cmd = "adb shell rm -rf  "+dir;

    qDebug()<<"CurentDir:"+cmd;
    ret = mfind.SendCmd(cmd,1);

    if(ret.indexOf("Read-only")>0)
    {
         QMessageBox::information(this,"warning", dir+" is Read-only file system");
         return;
    }

    if(ret.isEmpty())
    {
         QMessageBox::information(this,"warning", CurentDirPath+"删除成功");
           ui->list_Boxfile->clear();
         UpdateDir(CurentDirPath);
         return;
    }




//    if(ret.indexOf("File exists")>0)
//    {
//         QMessageBox::information(this,"warning", dir+"已经存在");
//         return;
//    }


    qDebug()<<"ret:"+ret;

}

//log 确定按钮
void MainWindow::on_cmd_confirm_clicked()
{
        QString cmd ,addcmd;
        QString pid,degree,tag;
        pid = ui->PID->text();
        degree = ui->degree->currentText();
        tag = ui->TAG->text();


        if(tag.isEmpty()) tag = "*";

        cmd = ui->cmd_display->toPlainText();


        if(PreCMD.compare(tag +":"+degree)==0)
        {
            addcmd = "";
        }
        else
        {
            addcmd = " "+tag +":"+degree;
        }
        PreCMD = tag +":"+degree;

        if(cmd.isEmpty())
        {
            cmd = "logcat -v time " + addcmd;
        }
        else
        {
            cmd =cmd + addcmd;
        }

        int index = cmd.indexOf("*:");
        if(index < 0)
        {
            cmd = cmd +" *:S";
        }

        ui->cmd_display->setText(cmd);
        qDebug()<<"log命令："+cmd;

}
//log 清除命令框里的命令
void MainWindow::on_clean_cmd_clicked()
{
    ui->cmd_display->clear();
    ui->TAG->clear();
    PreCMD.clear();
    ui->PID->clear();
}
//一键更新
void MainWindow::on_fresh_clicked()
{



    FastbootFileName.clear();
    KernelFileName.clear();

    QString cmd,ret;

    QString fastboot = ui->label_pp->text();
    QString kenral = ui->label_op->text();

    //= mSendTread->ret;
     if(fastboot.isEmpty()&&kenral.isEmpty())
     {

          QMessageBox::information(this,"提示","没有路径，请重新输入路径");
          return ;

     }


     KernelFileName =  kenral;
     FastbootFileName = fastboot;

     int index = KernelFileName.lastIndexOf("/");
     KernelFileName = KernelFileName.right(KernelFileName.length()-index-1);

     index = FastbootFileName.lastIndexOf("/");
     FastbootFileName = FastbootFileName.right(FastbootFileName.length()-index-1);




     cmd  ="adb shell  ls /dev/block/platform ";
     ret = mfind.SendCmd(cmd,1).trimmed().simplified();
     CPU_SOC = ret;


      qDebug()<<"KernelFileName:"+KernelFileName;
      qDebug()<<"FastbootFileName:"+FastbootFileName;







    ui->fresh->setEnabled(false);

    if((!fastboot.isEmpty())&&kenral.isEmpty()){//更新fastboot
          //弹出窗口
        QMessageBox::information(this,"提示","在更新fastboot中，请不要进行其他操作");
        QString cmd = "adb push " +fastboot+ " /data/";

        uploadFileStatus = 1;

        PrintANDSendCMD = true;

        CMD_TYPE =CMD_ADB_UPLOAD_FILE;
        emit StartSendCMD(cmd , 0);
        m_pTimer->setInterval(500);
        Timer_Thread->start();
    }
    if((!kenral.isEmpty())&&fastboot.isEmpty()){//更新内核
        //弹出窗口
      QMessageBox::information(this,"提示","在更新内核中，请不要进行其他操作");
        QString cmd = "adb push " +kenral+ " /data/";
              qDebug()<<"第1条命令："+cmd;


              uploadFileStatus = 2;

              PrintANDSendCMD = true;

              CMD_TYPE =CMD_ADB_UPLOAD_FILE;
              emit StartSendCMD(cmd , 0);
              m_pTimer->setInterval(500);
              Timer_Thread->start();
    }
    if(!fastboot.isEmpty()&&(!kenral.isEmpty())){
        //弹出窗口
         QMessageBox::information(this,"提示","在更新fastboot和内核的过程中，请不要进行其他操作");

         QString cmd = "adb push " +fastboot+ " /data/";

         CMD_TYPE =CMD_ADB_UPLOAD_FILE;
         uploadFileStatus = 3;

         kernelfastboot =false;
         PrintANDSendCMD = true;

         CMD_TYPE =CMD_ADB_UPLOAD_FILE;

         emit StartSendCMD(cmd , 0);

         m_pTimer->setInterval(500);
         Timer_Thread->start();

    }

       kernelfastboot =false;



}
//更新fastboot
void MainWindow::on_pushButton_4_clicked()
{

       QString file = QFileDialog::getOpenFileName(this,"select one file","",tr("*.img"));
       ui->label_pp->setText(file);
}

void MainWindow::on_pushButton_5_clicked()
{

    QString file = QFileDialog::getOpenFileName(this,"select one file","",tr("*.img"));
    ui->label_op->setText(file);

}

//停止 log
void MainWindow::on_pushButton_3_clicked()
{



    //    qDebug()<<"12311111";
     //     emit StopLog();

        if(MyLogPid.isEmpty())
        {
           QMessageBox::information(this,"提示","已经停止");
           return;
        }
        QString cmd = "adb shell kill "+MyLogPid;
        qDebug()<<"cmd :   "+cmd;
        mfind.SendCmd(cmd,1);

        MyLogPid.clear();










}
   //停止打印内核
void MainWindow::on_pushButton_6_clicked()
{
    mKmsgThread->status = true;
}
//向上
void MainWindow::on_up_bnt_clicked()
{
    QString cmd;
    cmd = "adb shell input keyevent 19";
    mfind.SendCmd(cmd,1);
    qDebug()<<"up:  "+cmd;
}
//向下
void MainWindow::on_down_bnt_clicked()
{
    QString cmd;
    cmd = "adb shell input keyevent 20";
    mfind.SendCmd(cmd,1);
    qDebug()<<"Down  "+cmd;
}
//向左
void MainWindow::on_left_bnt_clicked()
{
    QString cmd;
    cmd = "adb shell input keyevent 22";
    mfind.SendCmd(cmd,1);
    qDebug()<<"right:  "+cmd;
}
//向右
void MainWindow::on_right_bnt_clicked()
{
    QString cmd;
    cmd = "adb shell input keyevent 21";
    mfind.SendCmd(cmd,1);
    qDebug()<<"Left:  "+cmd;
}
//确定
void MainWindow::on_ok_bnt_clicked()
{
    QString cmd;
    cmd = "adb shell input keyevent 23";
    mfind.SendCmd(cmd,1);
    qDebug()<<"ok:  "+cmd;
}
//返回
void MainWindow::on_back_bnt_clicked()
{
    QString cmd;
    cmd = "adb shell input keyevent 4";
    mfind.SendCmd(cmd,1);
    qDebug()<<"ok:  "+cmd;
}

void MainWindow::on_chooseJpegPath_clicked()
{
    QString filename =QFileDialog::getExistingDirectory(this,tr("Open Directory"), "/home",
                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(filename.isEmpty())
        return;
    ui->JpegPath->setText(filename);

    JPegPath  =  ui->JpegPath->text();


}

void MainWindow::GetLogPiD()
{

   // connect(this,SIGNAL(StopLog()),mLogThread->pLogProcess,SLOT(terminate()));

    sleep(50);
    MyLogPid.clear();
    qDebug()<<"dsfsdf";

    QStringList templist,LogPidList;

    QString ret =   mfind.SendCmd("adb shell ps | grep logcat",1);

    if(ret.isEmpty())
    {
        qDebug()<<"no logcat pid";
        return;
    }

    LogPidList = ret.split("root");



    int index;

    for(int i=0;i<LogPidList.size();i++)
    {
        QString  temp = LogPidList.at(i).trimmed().simplified();
        index  =  temp.indexOf(" ");
        temp = temp.left(index).trimmed();
        if(mLogThread->pidlist.indexOf(temp)<0)
        {
            templist<<temp;
        }
        qDebug()<<"LogPid: "+temp;
    }

    if(templist.size()>1)
    {
         MyLogPid.clear();
         QMessageBox::information(this,"error","50ms有其他logcat进程出现,Logcat不能停止，关闭软件后才能停止");
    }
    else if(templist.size()==1)
    {
         MyLogPid = templist.at(0);
         qDebug()<<"MyLogPid: "+MyLogPid;
    }
    else if(templist.size()==0)
    {
         MyLogPid.clear();
    }


}

void MainWindow::on_remount_clicked()
{
    QString cmd,ret;
    cmd = "adb remount";
    ret = mfind.SendCmd(cmd,1);
    if(ret.indexOf("succee")>0)
    {
        ui->file_state->setText("挂载成功");
    }
    else
    {
         ui->file_state->setText("挂载失败");
    }
}

void MainWindow::IpPort()
{

    QStringList temlist;
    QString dir =  QCoreApplication::applicationDirPath()+"/defaut.txt";
    defautarg = new QFile(dir);
    if (defautarg->open(QIODevice ::ReadWrite))
    {
        QByteArray line = defautarg->readLine();
        QString str(line);
        qDebug()<< str;
        temlist = str.split(":");
    }

    ui->iptext->setText(temlist.at(0).trimmed().simplified());
    ui->port->setText(temlist.at(1).trimmed().simplified());




}

void MainWindow::on_search_clicked()
{

    QString search_item = ui->search_text->text();
    QFileIconProvider icon_provider;
    QString cmd,ret,add;
    QStringList dirlist;
    int start,end;

    ui->list_Boxfile->clear();

  //  Chosendir = Chosendir.left(Chosendir.length()-1);

    cmd = "adb shell ls "+CurentDirPath+" -F";

    qDebug()<<"cmd: "+CurentDirPath;

    ret = mfind.SendCmd(cmd,1);
    dirlist = ret.split("\n");



   for(int i=0;i<dirlist.size()-1;i++)
   {
      // qDebug()<<dirlist.at(i);
       int SpaceIndex = dirlist.at(i).indexOf(" ");

     //  qDebug()<<"space ；"+QString::number(SpaceIndex,10);

       if(dirlist.at(i).indexOf("No such")>0)
       {
           continue;
          //qDebug()<<"continus";
       }

       if(SpaceIndex==1)
       {
           if(dirlist.at(i).left(1).compare("d")==0)
           {
             //  qDebug()<<"dir";
               add = "/";


           }
           else
           {
             //  qDebug()<<"dir";
               add = "";
           }
       }
       else
       {
           if(dirlist.at(i).left(2).compare("ld")==0)
           {
             //  qDebug()<<"dir";
               add = "/";
           }
           else
           {
             //  qDebug()<<"dir";
               add = "";
           }
       }


       start = dirlist.at(i).indexOf(" ");
       end = dirlist.at(i).indexOf("\n");

       QString temp  =  dirlist.at(i).mid(start+1,end-2).trimmed().simplified()+add;


      // qDebug()<<"ccccc :"+temp.left(search_item.length())+search_item;

       if(temp.left(search_item.length()).compare(search_item)==0)
       {


           if(add.compare("/"))
           {
               QIcon icon = icon_provider.icon(QFileIconProvider::File);

               QListWidgetItem *item = new QListWidgetItem(icon,temp);
               ui->list_Boxfile->addItem(item);
           }
           else
           {
               QIcon icon = icon_provider.icon(QFileIconProvider::Folder);

               QListWidgetItem *item = new QListWidgetItem(icon,temp);
               ui->list_Boxfile->addItem(item);
           }
       }








     // qDebug()<<"temp:"+temp;

       CurentDir<<temp;


   }

}

void MainWindow::on_Refresh_clicked()
{
    UpdateDir(CurentDirPath);
}
