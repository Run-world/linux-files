#ifndef REMOTECONTRL_H
#define REMOTECONTRL_H

#include <QDialog>
#include <QWidget>
#include "mainwindow.h"
#include "adb_find.h"

namespace Ui {
class RemoteContrl;
}

class RemoteContrl : public QDialog
{
    Q_OBJECT

public:
    explicit RemoteContrl(QWidget *parent = 0);
    ~RemoteContrl();

private slots:
    void on_Up_pushButton_clicked();

    void on_Left_pushButton_clicked();

    void on_Down_pushButton_clicked();

    void on_Right_pushButton_clicked();

    void on_Ok_pushButton_clicked();


    void on_Home_pushButton_clicked();

    void on_Back_pushButton_clicked();

    void on_VolumeDown_pushButton_clicked();

    void on_VolumeUp_pushButton_clicked();

    void on_Power_pushButton_clicked();

    void on_Silence_pushButton_clicked();

    void on_one_clicked();

    void on_two_clicked();

    void on_three_clicked();

    void on_four_clicked();

    void on_five_clicked();

    void on_six_clicked();

    void on_seven_clicked();

    void on_eight_clicked();

    void on_nine_clicked();

    void on_yello_clicked();

    void on_del_clicked();

    void on_space_clicked();

private:
    Ui::RemoteContrl *ui;
    adb_find Remote;
    bool Pow_status;

};

#endif // REMOTECONTRL_H
