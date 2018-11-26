/********************************************************************************
** Form generated from reading UI file 'remotecontrl.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REMOTECONTRL_H
#define UI_REMOTECONTRL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_RemoteContrl
{
public:
    QVBoxLayout *verticalLayout_5;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_10;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *Silence_pushButton;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_9;
    QPushButton *Power_pushButton;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *VolumeUp_pushButton;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer_10;
    QPushButton *VolumeDown_pushButton;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *Up_pushButton;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *Left_pushButton;
    QPushButton *Ok_pushButton;
    QPushButton *Right_pushButton;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *Down_pushButton;
    QSpacerItem *horizontalSpacer_6;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_9;
    QPushButton *Home_pushButton;
    QSpacerItem *horizontalSpacer_7;
    QSpacerItem *horizontalSpacer_8;
    QPushButton *Back_pushButton;
    QHBoxLayout *horizontalLayout;
    QPushButton *one;
    QPushButton *two;
    QPushButton *three;
    QPushButton *del;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *four;
    QPushButton *five;
    QPushButton *six;
    QPushButton *yello;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *seven;
    QPushButton *eight;
    QPushButton *nine;
    QPushButton *space;

    void setupUi(QDialog *RemoteContrl)
    {
        if (RemoteContrl->objectName().isEmpty())
            RemoteContrl->setObjectName(QStringLiteral("RemoteContrl"));
        RemoteContrl->resize(362, 450);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(RemoteContrl->sizePolicy().hasHeightForWidth());
        RemoteContrl->setSizePolicy(sizePolicy);
        RemoteContrl->setMinimumSize(QSize(0, 0));
        RemoteContrl->setStyleSheet(QStringLiteral("background-image: url(:/image/bkg1.jpg);"));
        verticalLayout_5 = new QVBoxLayout(RemoteContrl);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        groupBox = new QGroupBox(RemoteContrl);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setStyleSheet(QString::fromUtf8("color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(85, 85, 255, 255), stop:1 rgba(85, 255, 255, 255));\n"
"font: 75 16pt \"\346\245\267\344\275\223\";"));
        groupBox->setAlignment(Qt::AlignCenter);
        horizontalLayout_10 = new QHBoxLayout(groupBox);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        Silence_pushButton = new QPushButton(groupBox);
        Silence_pushButton->setObjectName(QStringLiteral("Silence_pushButton"));
        Silence_pushButton->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));

        horizontalLayout_4->addWidget(Silence_pushButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_9);

        Power_pushButton = new QPushButton(groupBox);
        Power_pushButton->setObjectName(QStringLiteral("Power_pushButton"));
        Power_pushButton->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));

        horizontalLayout_4->addWidget(Power_pushButton);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        VolumeUp_pushButton = new QPushButton(groupBox);
        VolumeUp_pushButton->setObjectName(QStringLiteral("VolumeUp_pushButton"));
        VolumeUp_pushButton->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));

        horizontalLayout_5->addWidget(VolumeUp_pushButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_10);

        VolumeDown_pushButton = new QPushButton(groupBox);
        VolumeDown_pushButton->setObjectName(QStringLiteral("VolumeDown_pushButton"));
        VolumeDown_pushButton->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));

        horizontalLayout_5->addWidget(VolumeDown_pushButton);


        verticalLayout->addLayout(horizontalLayout_5);


        verticalLayout_4->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_3);

        Up_pushButton = new QPushButton(groupBox);
        Up_pushButton->setObjectName(QStringLiteral("Up_pushButton"));
        Up_pushButton->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));

        horizontalLayout_7->addWidget(Up_pushButton);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_4);


        verticalLayout_2->addLayout(horizontalLayout_7);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        Left_pushButton = new QPushButton(groupBox);
        Left_pushButton->setObjectName(QStringLiteral("Left_pushButton"));
        Left_pushButton->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));

        horizontalLayout_6->addWidget(Left_pushButton);

        Ok_pushButton = new QPushButton(groupBox);
        Ok_pushButton->setObjectName(QStringLiteral("Ok_pushButton"));
        Ok_pushButton->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));

        horizontalLayout_6->addWidget(Ok_pushButton);

        Right_pushButton = new QPushButton(groupBox);
        Right_pushButton->setObjectName(QStringLiteral("Right_pushButton"));
        Right_pushButton->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));

        horizontalLayout_6->addWidget(Right_pushButton);


        verticalLayout_2->addLayout(horizontalLayout_6);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_5);

        Down_pushButton = new QPushButton(groupBox);
        Down_pushButton->setObjectName(QStringLiteral("Down_pushButton"));
        Down_pushButton->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));

        horizontalLayout_8->addWidget(Down_pushButton);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_6);


        verticalLayout_2->addLayout(horizontalLayout_8);


        verticalLayout_4->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        Home_pushButton = new QPushButton(groupBox);
        Home_pushButton->setObjectName(QStringLiteral("Home_pushButton"));
        Home_pushButton->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));

        horizontalLayout_9->addWidget(Home_pushButton);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_7);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_8);

        Back_pushButton = new QPushButton(groupBox);
        Back_pushButton->setObjectName(QStringLiteral("Back_pushButton"));
        Back_pushButton->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));

        horizontalLayout_9->addWidget(Back_pushButton);


        verticalLayout_3->addLayout(horizontalLayout_9);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        one = new QPushButton(groupBox);
        one->setObjectName(QStringLiteral("one"));
        one->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));

        horizontalLayout->addWidget(one);

        two = new QPushButton(groupBox);
        two->setObjectName(QStringLiteral("two"));
        two->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));

        horizontalLayout->addWidget(two);

        three = new QPushButton(groupBox);
        three->setObjectName(QStringLiteral("three"));
        three->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));

        horizontalLayout->addWidget(three);

        del = new QPushButton(groupBox);
        del->setObjectName(QStringLiteral("del"));
        del->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));

        horizontalLayout->addWidget(del);


        verticalLayout_3->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        four = new QPushButton(groupBox);
        four->setObjectName(QStringLiteral("four"));
        four->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));

        horizontalLayout_2->addWidget(four);

        five = new QPushButton(groupBox);
        five->setObjectName(QStringLiteral("five"));
        five->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));

        horizontalLayout_2->addWidget(five);

        six = new QPushButton(groupBox);
        six->setObjectName(QStringLiteral("six"));
        six->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));

        horizontalLayout_2->addWidget(six);

        yello = new QPushButton(groupBox);
        yello->setObjectName(QStringLiteral("yello"));
        yello->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));

        horizontalLayout_2->addWidget(yello);


        verticalLayout_3->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        seven = new QPushButton(groupBox);
        seven->setObjectName(QStringLiteral("seven"));
        seven->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));

        horizontalLayout_3->addWidget(seven);

        eight = new QPushButton(groupBox);
        eight->setObjectName(QStringLiteral("eight"));
        eight->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));

        horizontalLayout_3->addWidget(eight);

        nine = new QPushButton(groupBox);
        nine->setObjectName(QStringLiteral("nine"));
        nine->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));

        horizontalLayout_3->addWidget(nine);

        space = new QPushButton(groupBox);
        space->setObjectName(QStringLiteral("space"));
        space->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));

        horizontalLayout_3->addWidget(space);


        verticalLayout_3->addLayout(horizontalLayout_3);


        verticalLayout_4->addLayout(verticalLayout_3);


        horizontalLayout_10->addLayout(verticalLayout_4);


        verticalLayout_5->addWidget(groupBox);


        retranslateUi(RemoteContrl);

        QMetaObject::connectSlotsByName(RemoteContrl);
    } // setupUi

    void retranslateUi(QDialog *RemoteContrl)
    {
        RemoteContrl->setWindowTitle(QApplication::translate("RemoteContrl", "\346\250\241\346\213\237\351\201\245\346\216\247\345\231\250", 0));
        groupBox->setTitle(QApplication::translate("RemoteContrl", "\346\250\241\346\213\237\351\201\245\346\216\247\345\231\250", 0));
        Silence_pushButton->setText(QApplication::translate("RemoteContrl", "\351\235\231\351\237\263", 0));
        Power_pushButton->setText(QApplication::translate("RemoteContrl", "\347\224\265\346\272\220", 0));
        VolumeUp_pushButton->setText(QApplication::translate("RemoteContrl", "\351\237\263\351\207\217-", 0));
        VolumeDown_pushButton->setText(QApplication::translate("RemoteContrl", "\351\237\263\351\207\217+", 0));
        Up_pushButton->setText(QApplication::translate("RemoteContrl", "\345\220\221\344\270\212", 0));
        Left_pushButton->setText(QApplication::translate("RemoteContrl", "\345\220\221\345\267\246", 0));
        Ok_pushButton->setText(QApplication::translate("RemoteContrl", "\347\241\256\345\256\232", 0));
        Right_pushButton->setText(QApplication::translate("RemoteContrl", "\345\220\221\345\217\263", 0));
        Down_pushButton->setText(QApplication::translate("RemoteContrl", "\345\220\221\344\270\213", 0));
        Home_pushButton->setText(QApplication::translate("RemoteContrl", "\344\270\273\351\241\265", 0));
        Back_pushButton->setText(QApplication::translate("RemoteContrl", "\350\277\224\345\233\236", 0));
        one->setText(QApplication::translate("RemoteContrl", "1", 0));
        two->setText(QApplication::translate("RemoteContrl", "2", 0));
        three->setText(QApplication::translate("RemoteContrl", "3", 0));
        del->setText(QApplication::translate("RemoteContrl", "\351\200\200\346\240\274", 0));
        four->setText(QApplication::translate("RemoteContrl", "4", 0));
        five->setText(QApplication::translate("RemoteContrl", "5", 0));
        six->setText(QApplication::translate("RemoteContrl", "6", 0));
        yello->setText(QApplication::translate("RemoteContrl", "0", 0));
        seven->setText(QApplication::translate("RemoteContrl", "7", 0));
        eight->setText(QApplication::translate("RemoteContrl", "8", 0));
        nine->setText(QApplication::translate("RemoteContrl", "9", 0));
        space->setText(QApplication::translate("RemoteContrl", "\347\251\272\346\240\274", 0));
    } // retranslateUi

};

namespace Ui {
    class RemoteContrl: public Ui_RemoteContrl {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REMOTECONTRL_H
