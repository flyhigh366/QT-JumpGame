/********************************************************************************
** Form generated from reading UI file 'game.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAME_H
#define UI_GAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_game
{
public:

    void setupUi(QDialog *game)
    {
        if (game->objectName().isEmpty())
            game->setObjectName("game");
        game->resize(670, 527);
        game->setStyleSheet(QString::fromUtf8("\n"
"QDialog {\n"
"border-image: url(:/background3.png) 0 0 0 0 stretch stretch;\n"
"}"));

        retranslateUi(game);

        QMetaObject::connectSlotsByName(game);
    } // setupUi

    void retranslateUi(QDialog *game)
    {
        game->setWindowTitle(QCoreApplication::translate("game", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class game: public Ui_game {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAME_H
