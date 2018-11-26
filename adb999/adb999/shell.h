#ifndef SHELL_H
#define SHELL_H

#include "adb_find.h"


#include  <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include <QObject>

class shell : public QObject
{
    Q_OBJECT
public:
    explicit shell(QObject *parent = 0);
    ~shell();
    adb_find *Shell_SendCMD;
    QString shellcmd;

signals:

public slots:


 void   Sysshell(void);
};

#endif // SHELL_H
