/********************************************************************************
** Form generated from reading UI file 'gamemenu.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMEMENU_H
#define UI_GAMEMENU_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QLineEdit *lineEdit;
    QPushButton *button_scoresheet;
    QPushButton *button_end;
    QPushButton *button_begin;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName("Dialog");
        Dialog->resize(635, 500);
        Dialog->setMaximumSize(QSize(635, 16777215));
        Dialog->setStyleSheet(QString::fromUtf8("QDialog{\n"
"    background-image:url(:/new/prefix2/background.jpg);\n"
"    background-size:100% 100%;\n"
"    background-repeat:no-repeat;\n"
"}"));
        lineEdit = new QLineEdit(Dialog);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(10, 30, 621, 86));
        QFont font;
        font.setFamilies({QString::fromUtf8("\346\226\260\345\256\213\344\275\223")});
        font.setPointSize(70);
        font.setBold(false);
        font.setItalic(false);
        lineEdit->setFont(font);
        lineEdit->setStyleSheet(QString::fromUtf8("color:rgb(116, 123, 255);\n"
"font: 70pt \"\346\226\260\345\256\213\344\275\223\";"));
        button_scoresheet = new QPushButton(Dialog);
        button_scoresheet->setObjectName("button_scoresheet");
        button_scoresheet->setGeometry(QRect(450, 220, 171, 91));
        QFont font1;
        font1.setPointSize(25);
        button_scoresheet->setFont(font1);
        button_scoresheet->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	border:none;\n"
"    background:url(:/new/prefix2/button.png) ;\n"
"	border-radius:25px;\n"
"  color:rgb(128, 111, 255)\n"
"}\n"
"QPushButton:hover {\n"
"     border:none;\n"
"    background:url(:/new/prefix2/hover.png);\n"
"	border-radius:25px;\n"
"}\n"
"QPushButton:pressed{\n"
"   border:none;\n"
"    background:url(:/new/prefix2/button.png) ;\n"
"	border-radius:25px;\n"
"\n"
"}"));
        button_end = new QPushButton(Dialog);
        button_end->setObjectName("button_end");
        button_end->setGeometry(QRect(240, 220, 191, 91));
        button_end->setFont(font1);
        button_end->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	border:none;\n"
"    background:url(:/new/prefix2/button.png) ;\n"
"	border-radius:25px;\n"
"  color:rgb(128, 111, 255)\n"
"}\n"
"QPushButton:hover {\n"
"     border:none;\n"
"    background:url(:/new/prefix2/hover.png);\n"
"	border-radius:25px;\n"
"}\n"
"QPushButton:pressed{\n"
"   border:none;\n"
"    background:url(:/new/prefix2/button.png) ;\n"
"	border-radius:25px;\n"
"\n"
"}"));
        button_begin = new QPushButton(Dialog);
        button_begin->setObjectName("button_begin");
        button_begin->setGeometry(QRect(10, 220, 201, 91));
        button_begin->setFont(font1);
        button_begin->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	border:none;\n"
"    background:url(:/new/prefix2/button.png) ;\n"
"	border-radius:25px;\n"
"    color:rgb(128, 111, 255)\n"
"     \n"
"}\n"
"QPushButton:hover {\n"
"     border:none;\n"
"    background:url(:/new/prefix2/hover.png);\n"
"	border-radius:25px;\n"
"}\n"
"QPushButton:pressed{\n"
"   border:none;\n"
"    background:url(:/new/prefix2/button.png) ;\n"
"	border-radius:25px;\n"
"\n"
"}"));
        button_end->raise();
        lineEdit->raise();
        button_begin->raise();
        button_scoresheet->raise();

        retranslateUi(Dialog);
        QObject::connect(button_end, &QPushButton::clicked, Dialog, qOverload<>(&QDialog::reject));
        QObject::connect(button_begin, &QPushButton::clicked, Dialog, qOverload<>(&QDialog::accept));

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        lineEdit->setText(QCoreApplication::translate("Dialog", "\350\267\263\344\270\200\350\267\263\345\260\217\346\270\270\346\210\217", nullptr));
        button_scoresheet->setText(QCoreApplication::translate("Dialog", "\346\270\270\346\210\217\350\256\260\345\275\225", nullptr));
        button_end->setText(QCoreApplication::translate("Dialog", "\347\273\223\346\235\237\346\270\270\346\210\217", nullptr));
        button_begin->setText(QCoreApplication::translate("Dialog", "\345\274\200\345\247\213\346\270\270\346\210\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEMENU_H
