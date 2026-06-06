#include "gamemenu.h"
#include "ui_gamemenu.h"
#include"gameview.h"
#include "leaderboard.h"
#include "achievements.h"
#include"setboard.h"
#include<QFile>
#include<QDebug>
#include <QApplication>
#include <QSettings> // 引入 QSettings

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    // Widget/Dialog构造函数内
    // ==========================================
    // 批量绑定按键音效 (记得在文件最上方 #include <QSoundEffect>)
    // ==========================================
    QSoundEffect *clickSound = new QSoundEffect(this);
    clickSound->setSource(QUrl("qrc:/sounds/click.wav"));
    clickSound->setVolume(1.0f);

    // 黑科技：一网打尽当前窗口里的所有 QPushButton！
    QList<QPushButton*> allButtons = this->findChildren<QPushButton*>();
    for (QPushButton *btn : allButtons) {
        // 当任何按钮被点击时，都播放这个音效
        connect(btn, &QPushButton::clicked, clickSound, &QSoundEffect::play);
    }
    QSettings settings("MyGame", "DDL_Survival");
    m_skin = settings.value("SelectedSkin", 0).toInt();

}

Dialog::~Dialog()
{
    delete ui;
}
void Dialog::saveSkin() {
    // 将当前皮肤保存到本地，下次打开自动生效
    QSettings settings("MyGame", "DDL_Survival");
    settings.setValue("SelectedSkin", m_skin);

    // 你可以在这里加个 QMessageBox::information 提示玩家“皮肤更换成功！”
}
void Dialog:: Showme(){
    show();
}

void Dialog::on_button_scoresheet_clicked()
{
    Leaderboard lb;
    int ret = lb.exec();
    if (ret == QDialog::Accepted) {
        this->accept();
    }
}


void Dialog::on_pushButton_clicked()
{
    setboard sb;
    int ret = sb.exec();

    // 如果在设置面板点击了“开始游戏” (Accepted)
    if (ret == QDialog::Accepted) {
        // --- 修改点：原来的 m_hp = sb.getHp(); 那些全删掉，因为 main.cpp 会自动去读档 ---
        // 直接关闭主菜单，进入游戏
        this->accept();
    }
}

// 记得在 gamemenu.h 里也声明一下 void on_btn_achievements_clicked();
void Dialog::on_btn_achievements_clicked()
{
    achievements achBoard(this);



    // 用 exec() 弹出，但我们不在乎它返回啥
    int ret=achBoard.exec();
    // 如果在设置面板点击了“开始游戏” (Accepted)
    if (ret == QDialog::Accepted) {
        // --- 修改点：原来的 m_hp = sb.getHp(); 那些全删掉，因为 main.cpp 会自动去读档 ---
        // 直接关闭主菜单，进入游戏
        this->accept();
    }

}

void Dialog::on_btn_skinDefault_clicked() { m_skin = 0; saveSkin(); }
void Dialog::on_btn_skinDog_clicked()     { m_skin = 1; saveSkin(); }
void Dialog::on_btn_skinCat_clicked()     { m_skin = 2; saveSkin(); }
void Dialog::on_btn_skinPanda_clicked()   { m_skin = 3; saveSkin(); }

