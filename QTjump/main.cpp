
#include <QApplication>
#include<QObject>
#include "gameview.h"
#include "mainmenu.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainMenu* menu = new MainMenu;
    GameView* game = nullptr;

    QObject::connect(menu,&MainMenu::startGame,[&](){
        menu->hide();
        game = new GameView;
        game->show();
        QObject::connect(game,&GameView::backToMenu,[&](){
            delete game; game=nullptr;
            menu->show();
        });
    });
    QObject::connect(menu,&MainMenu::quitApp,&a,&QApplication::quit);

    menu->show();
    return a.exec();
}