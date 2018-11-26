#include "remotecontrl.h"
#include "ui_remotecontrl.h"

RemoteContrl::RemoteContrl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RemoteContrl)
{
    ui->setupUi(this);
    Pow_status = false;    //电源状态
    Remote = adb_find();
//   ui->Ok_pushButton->setShortcut(Qt::Key_Return);   //OK->Enter
    ui->Back_pushButton->setShortcut(Qt::Key_Backspace);//Back->Backspace
    ui->one->setShortcut(Qt::Key_1);
    ui->two->setShortcut(Qt::Key_2);
    ui->three->setShortcut(Qt::Key_3);
    ui->four->setShortcut(Qt::Key_4);
    ui->five->setShortcut(Qt::Key_5);
    ui->six->setShortcut(Qt::Key_6);
    ui->seven->setShortcut(Qt::Key_7);
    ui->eight->setShortcut(Qt::Key_8);
    ui->nine->setShortcut(Qt::Key_9);
    ui->yello->setShortcut(Qt::Key_0);
    ui->space->setShortcut(Qt::Key_Space);

    this->setFixedSize(this->width(),this->height());
}

RemoteContrl::~RemoteContrl()
{
    delete ui;
}
//up 按钮
void RemoteContrl::on_Up_pushButton_clicked()
{
    QString cmd;
    cmd = "adb shell input keyevent 19";
    Remote.SendCmd(cmd,1);
    qDebug()<<"up:  "+cmd;
}

//Left 按钮
void RemoteContrl::on_Left_pushButton_clicked()
{
    QString cmd;
    cmd = "adb shell input keyevent 21";
    Remote.SendCmd(cmd,1);
    qDebug()<<"Left:  "+cmd;
}

void RemoteContrl::on_Down_pushButton_clicked()
{
    QString cmd;
    cmd = "adb shell input keyevent 20";
    Remote.SendCmd(cmd,1);
    qDebug()<<"Down  "+cmd;
}


void RemoteContrl::on_Right_pushButton_clicked()
{
    QString cmd;
    cmd = "adb shell input keyevent 22";
    Remote.SendCmd(cmd,1);
    qDebug()<<"right:  "+cmd;
}

void RemoteContrl::on_Ok_pushButton_clicked()
{
    QString cmd;
    cmd = "adb shell input keyevent 23";
    Remote.SendCmd(cmd,1);
    qDebug()<<"ok:  "+cmd;
}


void RemoteContrl::on_Home_pushButton_clicked()
{
    QString cmd;
    cmd = "adb shell input keyevent 3";
    Remote.SendCmd(cmd,1);
    qDebug()<<"ok:  "+cmd;
}

void RemoteContrl::on_Back_pushButton_clicked()
{
    QString cmd;
    cmd = "adb shell input keyevent 4";
    Remote.SendCmd(cmd,1);
    qDebug()<<"ok:  "+cmd;
}

void RemoteContrl::on_VolumeDown_pushButton_clicked()
{
    QString cmd;
    cmd = "adb shell input keyevent 25";
    Remote.SendCmd(cmd,1);
    qDebug()<<"ok:  "+cmd;
}

void RemoteContrl::on_VolumeUp_pushButton_clicked()
{
    QString cmd;
    cmd = "adb shell input keyevent 24";
    Remote.SendCmd(cmd,1);
    qDebug()<<"ok:  "+cmd;
}

void RemoteContrl::on_Power_pushButton_clicked()
{
    QString cmd;

    if(Pow_status == false)
    {
        cmd = "adb shell start";
        Pow_status = true;
    }
    else if(Pow_status == true)
    {
        cmd = "adb shell stop";
        Pow_status = false;
    }
    Remote.SendCmd(cmd,1);
    qDebug()<<"ok:  "+cmd;
}

void RemoteContrl::on_Silence_pushButton_clicked()
{
    QString cmd;
    cmd = "adb shell input keyevent 164";
    Remote.SendCmd(cmd,1);
    qDebug()<<"ok:  "+cmd;
}
//1按钮
void RemoteContrl::on_one_clicked()
{
    QString cmd;
    cmd = "adb shell input keyevent 8";
    Remote.SendCmd(cmd,1);
    qDebug()<<"1:  "+cmd;
}
//2按钮
void RemoteContrl::on_two_clicked()
{
    QString cmd;
    cmd = "adb shell input keyevent 9";
    Remote.SendCmd(cmd,1);
    qDebug()<<"1:  "+cmd;
}
//3按钮
void RemoteContrl::on_three_clicked()
{
    QString cmd;
    cmd = "adb shell input keyevent 10";
    Remote.SendCmd(cmd,1);
    qDebug()<<"1:  "+cmd;
}
//4按钮
void RemoteContrl::on_four_clicked()
{
    QString cmd;
    cmd = "adb shell input keyevent 11";
    Remote.SendCmd(cmd,1);
    qDebug()<<"1:  "+cmd;
}
//5按钮
void RemoteContrl::on_five_clicked()
{
    QString cmd;
    cmd = "adb shell input keyevent 12";
    Remote.SendCmd(cmd,1);
    qDebug()<<"1:  "+cmd;
}
//6按钮
void RemoteContrl::on_six_clicked()
{
    QString cmd;
    cmd = "adb shell input keyevent 13";
    Remote.SendCmd(cmd,1);
    qDebug()<<"1:  "+cmd;
}
//7按钮
void RemoteContrl::on_seven_clicked()
{
    QString cmd;
    cmd = "adb shell input keyevent 14";
    Remote.SendCmd(cmd,1);
    qDebug()<<"1:  "+cmd;
}
//8按钮
void RemoteContrl::on_eight_clicked()
{
    QString cmd;
    cmd = "adb shell input keyevent 15";
    Remote.SendCmd(cmd,1);
    qDebug()<<"1:  "+cmd;
}
//9按钮
void RemoteContrl::on_nine_clicked()
{
    QString cmd;
    cmd = "adb shell input keyevent 16";
    Remote.SendCmd(cmd,1);
    qDebug()<<"1:  "+cmd;
}
//0按钮
void RemoteContrl::on_yello_clicked()
{
    QString cmd;
    cmd = "adb shell input keyevent 7";
    Remote.SendCmd(cmd,1);
    qDebug()<<"1:  "+cmd;
}

//删除按钮
void RemoteContrl::on_del_clicked()
{
    QString cmd;
    cmd = "adb shell input keyevent 67";
    Remote.SendCmd(cmd,1);
    qDebug()<<"1:  "+cmd;
}
//空格键
void RemoteContrl::on_space_clicked()
{
    QString cmd;
    cmd = "adb shell input keyevent 62";
    Remote.SendCmd(cmd,1);
    qDebug()<<"1:  "+cmd;
}
