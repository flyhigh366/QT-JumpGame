#include "gamemenu.h"
#include "gameview.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    while (true)
    {
        Dialog menu;
        int ret=menu.exec();
        if (ret==QDialog::Rejected)
        {
            // 点退出 → 结束整个程序
            break;
        }


        // 进入游戏
        else if(ret ==QDialog::Accepted){

            int hp = menu.getHp();
            int time = menu.getTime();

            // 把参数传递给游戏核心场景
            // 把菜单里读取到的皮肤，传给 gameview
            int skin = menu.getSkin();
            gameview w(nullptr, hp, time, skin);

            // 游戏内的 backToMenu 信号触发时，关闭游戏窗口(reject)，循环会自动回到主菜单
            QObject::connect(&w, &gameview::backToMenu, &w, &gameview::reject);

            w.exec();// 游戏运行时的事件循环
        }

    }

    // 用户点击了"结束游戏"按钮或关闭窗口
    return 0;
}
