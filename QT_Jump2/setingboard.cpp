#include "setingboard.h"
#include "ui_setingboard.h"

Setingboard::Setingboard(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Setingboard::~Setingboard()
{
    delete ui;
}
