#ifndef LOGTHREAD_H
#define LOGTHREAD_H
#include "adb_find.h"

#include <QDebug>
#include <QString>
#include <QStringList>
#include <QObject>
#include <QThread>
#include  <winsock2.h>
#include <QProcess>

#pragma comment(lib, "ws2_32.lib")

class logthread : public QObject
{
    Q_OBJECT
public:
    explicit logthread(QObject *parent = 0);
    ~logthread();
    bool PrintFlag;
    QStringList retList;
    QStringList pidlist;
    bool status;
     bool stop;
    void LogProcessRound(QString,QString);

    QProcess *pLogProcess;

    QStrinetPidSingal();

public slots:
    void PrintLogCat(QString pid, QString cmd1, QString filename);
    void StopLogSlot();

   signals:
    GetPidSingal();

private:
    adb_find log_adb;
    adb_find pull_log;




};



#endif // LOGTHREAD_H
