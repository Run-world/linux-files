#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "remotecontrl.h"
#include "adb_find.h"
#include "mythread.h"
#include "logthread.h"
#include "sendthread.h"
#include "listenthread.h"

#include "listenthread.h"
#include "kmsgthread.h"
#include "shell.h"




#include <QMainWindow>
#include <QHostInfo>
#include <QDebug>
#include <QString>
#include <QProcess>
#include <QFileDialog>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QListWidget>
#include <QPixmap>
#include <QImage>
#include <QThread>
#include <QTime>
#include <QThread>
#include <QTimer>
#include <QFileIconProvider>
#include <QIcon>
#include <QScreen>
#include <QDateTime>




#include  <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

class RemoteContrl;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void Get_Box_Info();
    void sleep(unsigned int msec);
    int Listener(void);
    QString GetLocalIP();
    void ScanIP(void);
    void UpdateDir(QString curdir);

    enum CMD_LIST
    {
        CMD_ADB_CONNECT =0,
        CMD_ADB_UPLOAD_FILE,
        CMD_ADB_DOWNLOAD_FILE,
        CMD_ADB_UPLOAD_DIR,
        CMD_ADB_DOWNLOAD_DIR,
        CMD_ADB_INSTALL,
        CMD_ADB_REBOTT,
        CMD_ADB_UPLOAD_UPDATE,
        APP_IPADRESS,
        APP_GPU,
        APP_LIST,
    };




signals:
    void listItemClicked(int);
    int  startListener1();
    void StartPlayer(bool);
//    void StartPrint(QString pid,QString degree,QString tag,int status);
    void StartPrint(QString ,QString ,QString );
    void StartSendCMD(QString , int);
    void StartListen(void);
    void StartKmsg(QString fileroute,QString filename);
    void StopLog();



private slots:


    void timeOutSlot();
    void timeOutNetSlot();
    void PlayertimeOutNetSlot();


    void UpdateShootScreen(void);
    void singleClickSlot(QListWidgetItem *);
    void BoxFilesingleClickSlot(QListWidgetItem *);

    void DoubleClickSlot(QListWidgetItem *);
    void closeEvent(QCloseEvent *e);



    void on_connect_clicked();

    void on_disconnect_clicked();

    void on_pushButton_3_clicked();
    void on_pushButton_6_clicked();

    void on_curi_clicked();

    void on_ddr_clicked();

    void on_mache_clicked();

    void on_flash_clicked();

    void on_cpu_clicked();

    void on_GPU_clicked();

    void on_num_clicked();

    void on_mac_clicked();

    void on_fireware_clicked();

    void on_kernel_clicked();

    void on_SysVer_clicked();

    void on_print_clicked();

    void on_upload_clicked();

    void on_download_clicked();

    void on_upload_dir_clicked();

    void on_download_dir_clicked();

    void on_AppList_clicked();

    void on_Install_clicked();

    void on_uninstall_clicked();

    void on_start_clicked();

    void on_stop_clicked();

    void on_shootscreen_clicked();

    void on_player_clicked();

 // void on_send_clicked();

    void on_listen_clicked();

    void on_print_kmsg_clicked();



    void on_roteCotrul_pushButton_clicked();

    void on_Recovery1_pushButton_clicked();

    void on_Recovery2_pushButton_clicked();

    void on_Recovery3_pushButton_clicked();

    void on_RebootpushButton_clicked();



    void on_pushButton_2_clicked();

    void on_DownLoadFastboot_clicked();

    void on_DownLoadKernel_clicked();

    void on_App_GetIP_clicked();

    void on_Scan_clicked();

    void on_pushButton_clicked();

    void on_ScanBoxDir_clicked();

    void on_dir_back_clicked();

    void on_Create_Dir_clicked();

    void on_DeleteDirFile_clicked();

    void on_cmd_confirm_clicked();

    void on_clean_cmd_clicked();

    void on_fresh_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_up_bnt_clicked();

    void on_down_bnt_clicked();

    void on_left_bnt_clicked();

    void on_right_bnt_clicked();

    void on_ok_bnt_clicked();

    void on_back_bnt_clicked();

    void on_chooseJpegPath_clicked();
    void GetLogPiD();

    void on_remount_clicked();

    void on_search_clicked();

    void on_Refresh_clicked();


private:
    Ui::MainWindow *ui;
    QString package;

    void IpPort();
    QFile *defautarg;

    adb_find mfind;
    MyThread *PlayerThread;
      kmsgthread *mKmsgThread;
    logthread *mLogThread;
    shell  *mshell;


    QThread *thread5;
    QThread *thread;
    QThread *thread1;
    QThread *thread2;
   // QThread *thread3;
    QThread *thread4;


    QDateTime  datetime;


    bool kernelfastboot;
    bool packageAndChinese;

    QThread * Timer_Thread;
    QThread * Timer_Thread_Net;

    QThread * Timer_Thread_Player;
    QTimer *m_pTimer;
    QTimer *ListenTimer;
    QTimer *PlayerTimer;



    int FrameRate;
    bool playerflag;

    RemoteContrl *rc;
    sendthread *mSendTread;
    bool PrintANDSendCMD;


    CMD_LIST CMD_TYPE ;
    double SendCMDCount;

    bool Netsatus;
    bool SingleShoot;



    QString ZipName;
    QString MyIp;
    QString RemoteIp;
    QString ADBPath;

    QString LocalMyIp;
    QString LocalIP;
    QStringList CurentDir;
    QString CurentDirPath;
    QString ChosenDir;

    QString JPegPath;

    QString FastbootFileName;
    QString KernelFileName;
    QString CPU_SOC;

    QString ChosenFile;
    QString PreCMD;

    QString  uploadDir;
    QString  downroadDir;


    QStringList PackageList;
    QStringList AppNameList;
    QStringList StringList;
    QString MyLogPid;

    listenthread *mListenThread;
    int downloadFileStatus;
    int uploadFileStatus;
    QStringList CurIplist;

    int MaxHeight;
};













#endif // MAINWINDOW_H
