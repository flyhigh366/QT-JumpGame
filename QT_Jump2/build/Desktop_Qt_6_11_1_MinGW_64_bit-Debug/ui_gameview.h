/********************************************************************************
** Form generated from reading UI file 'gameview.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMEVIEW_H
#define UI_GAMEVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_gameview
{
public:
    QWidget *gameview_2;

    void setupUi(QDialog *gameview)
    {
        if (gameview->objectName().isEmpty())
            gameview->setObjectName("gameview");
        gameview->resize(1000, 1000);
        gameview->setStyleSheet(QString::fromUtf8("QDialog{\n"
"	border-image: url(:/new/prefix2/background2.png) 0 0 0 0 stretch stretch;\n"
"}"));
        gameview_2 = new QWidget(gameview);
        gameview_2->setObjectName("gameview_2");
        gameview_2->setGeometry(QRect(0, 0, 1000, 1000));

        retranslateUi(gameview);

        QMetaObject::connectSlotsByName(gameview);
    } // setupUi

    void retranslateUi(QDialog *gameview)
    {
        gameview->setWindowTitle(QCoreApplication::translate("gameview", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class gameview: public Ui_gameview {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEVIEW_H
