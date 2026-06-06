#include "achievements.h"
#include "ui_achievements.h"
#include <QSettings>
#include <QVBoxLayout>
#include <QFrame>
#include <QSoundEffect> // 负责音效类
#include <QUrl>         // 负责处理资源路径
#include <QLabel>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>
#include <QScrollArea>

achievements::achievements(QWidget *parent)
    : QDialog(parent), ui(new Ui::achievements)
{
    ui->setupUi(this);


}

achievements::~achievements()
{
    delete ui;
}
void achievements::on_btn_back_clicked()
{
    this->reject();
}


void achievements::on_btn_start_clicked()
{
    this->accept();
}

