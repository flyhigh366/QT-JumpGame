#include "gamemenu.h"
#include "ui_gamemenu.h"
#include "leaderboard.h"
#include <QPushButton>
#include <QSettings>
#include <QFile>
#include <QDebug>
#include <QApplication>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    // ===== 皮肤选择按钮（居中布局） =====
    int btnWidth = 140;
    int btnHeight = 40;
    int spacing = 30;
    int totalWidth = 3 * btnWidth + 2 * spacing;
    int startX = (this->width() - totalWidth) / 2;
    int yPos = 320;

    m_btnDog = new QPushButton("🐶 DDL战士小狗", this);
    m_btnCat = new QPushButton("🐱 平静卷王茂密", this);
    m_btnPanda = new QPushButton("🐼 苦命早八熊猫", this);

    m_btnDog->setGeometry(startX, yPos, btnWidth, btnHeight);
    m_btnCat->setGeometry(startX + btnWidth + spacing, yPos, btnWidth, btnHeight);
    m_btnPanda->setGeometry(startX + 2*(btnWidth + spacing), yPos, btnWidth, btnHeight);


    QString baseStyle = "QPushButton { border-radius: 8px; font-size: 14px; font-weight: bold; }";
    m_btnDog->setStyleSheet(baseStyle);
    m_btnCat->setStyleSheet(baseStyle);
    m_btnPanda->setStyleSheet(baseStyle);

    connect(m_btnDog, &QPushButton::clicked, this, &Dialog::selectSkinDog);
    connect(m_btnCat, &QPushButton::clicked, this, &Dialog::selectSkinCat);
    connect(m_btnPanda, &QPushButton::clicked, this, &Dialog::selectSkinPanda);


    int currentSkin = QSettings().value("ballSkin", 0).toInt();
    QString activeStyle = "background-color: #90EE90; border: 2px solid #228B22;";  // 浅绿+深绿边框
    if (currentSkin == 0) m_btnDog->setStyleSheet(baseStyle + activeStyle);
    else if (currentSkin == 1) m_btnCat->setStyleSheet(baseStyle + activeStyle);
    else if (currentSkin == 2) m_btnPanda->setStyleSheet(baseStyle + activeStyle);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::Showme(){
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

void Dialog::selectSkinDog()
{
    QSettings().setValue("ballSkin", 0);
    QSettings().sync();
    m_btnDog->setStyleSheet("background-color: lightgreen;");
    m_btnCat->setStyleSheet("");
    m_btnPanda->setStyleSheet("");
}

void Dialog::selectSkinCat()
{
    QSettings().setValue("ballSkin", 1);
    QSettings().sync();
    m_btnDog->setStyleSheet("");
    m_btnCat->setStyleSheet("background-color: lightgreen;");
    m_btnPanda->setStyleSheet("");
}

void Dialog::selectSkinPanda()
{
    QSettings().setValue("ballSkin", 2);
    QSettings().sync();
    m_btnDog->setStyleSheet("");
    m_btnCat->setStyleSheet("");
    m_btnPanda->setStyleSheet("background-color: lightgreen;");
}