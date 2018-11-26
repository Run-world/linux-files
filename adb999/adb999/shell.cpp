#include "shell.h"

shell::shell(QObject *parent) : QObject(parent)
{
    Shell_SendCMD = new adb_find();
    QString AppPath =    Shell_SendCMD->GetAPPPath();
    shellcmd = AppPath+"adb.exe shell";
    qDebug()<<"shell cmd :"+shellcmd;


}

shell::~shell()
{

}

void shell::Sysshell()
{


    QByteArray ba = shellcmd.toLatin1(); // must
    char *ch1 = ba.data();
    system(ch1);
}

