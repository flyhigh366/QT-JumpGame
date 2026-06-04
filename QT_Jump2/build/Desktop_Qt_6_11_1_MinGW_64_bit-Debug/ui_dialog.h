/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

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
    QPushButton *pushButton_2;
    QPushButton *pushButton;
    QPushButton *pushButton_3;

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
        pushButton_2 = new QPushButton(Dialog);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(240, 220, 191, 91));
        QFont font1;
        font1.setPointSize(25);
        pushButton_2->setFont(font1);
        pushButton_2->setStyleSheet(QString::fromUtf8("QPushButton{\n"
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
        pushButton = new QPushButton(Dialog);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(450, 220, 171, 91));
        pushButton->setFont(font1);
        pushButton->setStyleSheet(QString::fromUtf8("QPushButton{\n"
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
        pushButton_3 = new QPushButton(Dialog);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(10, 220, 201, 91));
        pushButton_3->setFont(font1);
        pushButton_3->setStyleSheet(QString::fromUtf8("QPushButton{\n"
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
        lineEdit->raise();
        pushButton_3->raise();

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        lineEdit->setText(QCoreApplication::translate("Dialog", "\350\267\263\344\270\200\350\267\263\345\260\217\346\270\270\346\210\217", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Dialog", "\347\273\223\346\235\237\346\270\270\346\210\217", nullptr));
        pushButton->setText(QCoreApplication::translate("Dialog", "\346\270\270\346\210\217\350\256\260\345\275\225", nullptr));
        pushButton_3->setText(QCoreApplication::translate("Dialog", "\345\274\200\345\247\213\346\270\270\346\210\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
