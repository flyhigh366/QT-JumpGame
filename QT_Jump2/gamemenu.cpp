#include "gamemenu.h"
#include "ui_gamemenu.h"
#include<QFile>
#include<QDebug>
#include <QApplication>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    // Widget/Dialog构造函数内

}

Dialog::~Dialog()
{
    delete ui;
}
void Dialog:: Showme(){
    show();
}











