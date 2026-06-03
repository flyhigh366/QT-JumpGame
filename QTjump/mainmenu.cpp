#include "mainmenu.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QFont>
#include "platform.h"

MainMenu::MainMenu(QWidget *parent):QWidget(parent)
{
    setFixedSize(GAME_W, GAME_H);
    setStyleSheet("background-color:#222222;");
    QVBoxLayout* lay = new QVBoxLayout(this);
    lay->setSpacing(40);
    lay->setContentsMargins(150,120,150,120);

    QLabel* title = new QLabel("小球跳跃闯关");
    title->setFont(QFont("微软雅黑",30,QFont::Bold));
    title->setStyleSheet("color:white;");
    title->setAlignment(Qt::AlignCenter);

    QPushButton* btnStart = new QPushButton("开始游戏");
    btnStart->setFixedHeight(50);
    btnStart->setFont(QFont("微软雅黑",14));
    btnStart->setStyleSheet("background:#00aa77;color:white;border-radius:6px;");

    QPushButton* btnQuit = new QPushButton("退出游戏");
    btnQuit->setFixedHeight(50);
    btnQuit->setFont(QFont("微软雅黑",14));
    btnQuit->setStyleSheet("background:#aa3333;color:white;border-radius:6px;");

    lay->addWidget(title);
    lay->addWidget(btnStart);
    lay->addWidget(btnQuit);

    connect(btnStart,&QPushButton::clicked,this,&MainMenu::startGame);
    connect(btnQuit,&QPushButton::clicked,this,&MainMenu::quitApp);
}