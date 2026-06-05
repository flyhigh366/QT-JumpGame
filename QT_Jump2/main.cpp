#include "gamemenu.h"
#include "gameview.h"
#include <QApplication>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("PKU");
    QCoreApplication::setApplicationName("QTJumpGame");
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
        gameview w;
        // 游戏返回 → 关闭游戏窗口，循环回到菜单
        QObject::connect(&w, &gameview::backToMenu, &w, &gameview::reject);

        w.exec(); // 游戏运行时的事件循环
        }

    }

    // 用户点击了"结束游戏"按钮或关闭窗口
    return 0;
}
