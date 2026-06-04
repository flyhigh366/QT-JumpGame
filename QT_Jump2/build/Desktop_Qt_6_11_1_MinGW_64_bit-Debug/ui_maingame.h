/********************************************************************************
** Form generated from reading UI file 'maingame.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINGAME_H
#define UI_MAINGAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_maingame
{
public:

    void setupUi(QDialog *maingame)
    {
        if (maingame->objectName().isEmpty())
            maingame->setObjectName("maingame");
        maingame->resize(400, 300);
        maingame->setStyleSheet(QString::fromUtf8("QDialog{\n"
"    \n"
"	border-image: url() 0 0 0 0 stretch stretch;\n"
"	border-image: url(:/new/prefix2/background2.png);\n"
"}"));

        retranslateUi(maingame);

        QMetaObject::connectSlotsByName(maingame);
    } // setupUi

    void retranslateUi(QDialog *maingame)
    {
        maingame->setWindowTitle(QCoreApplication::translate("maingame", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class maingame: public Ui_maingame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINGAME_H
