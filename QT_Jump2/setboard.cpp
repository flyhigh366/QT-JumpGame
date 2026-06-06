#include "setboard.h"
#include "ui_setboard.h"
#include <QSettings>
#include <QSoundEffect> // 负责音效类
#include <QUrl>         // 负责处理资源路径

setboard::setboard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setboard)
{
    ui->setupUi(this);


    ui->spinHp->setMinimum(3);
    ui->spinHp->setMaximum(99);
    ui->spinTime->setMinimum(30);
    ui->spinTime->setMaximum(300);
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

    // --- 修改点：打开面板时，直接读取本地存档的设置 ---
    QSettings settings("MyGame", "DDL_Survival");
    ui->spinHp->setValue(settings.value("PlayerHp", 3).toInt());
    ui->spinTime->setValue(settings.value("GameTime", 150).toInt());

    connect(ui->btnStart, &QPushButton::clicked, this, &setboard::on_btnStart_clicked);
    connect(ui->btnReturn, &QPushButton::clicked, this, &setboard::on_btnReturn_clicked);
}

setboard::~setboard()
{
    delete ui;
}

void setboard::on_btnStart_clicked()
{
    ui->spinHp->clearFocus();
    ui->spinTime->clearFocus();
    // --- 修改点：保存设置到本地 ---
    QSettings settings("MyGame", "DDL_Survival");
    settings.setValue("PlayerHp", ui->spinHp->value());
    settings.setValue("GameTime", ui->spinTime->value());

    this->accept();
}

void setboard::on_btnReturn_clicked()
{
    // --- 修改点：即使用户点返回，也把他们调好的值保存下来！ ---
    ui->spinHp->clearFocus();
    ui->spinTime->clearFocus();
    QSettings settings("MyGame", "DDL_Survival");
    settings.setValue("PlayerHp", ui->spinHp->value());
    settings.setValue("GameTime", ui->spinTime->value());

    this->reject();
}
