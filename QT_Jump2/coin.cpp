#include "coin.h"
#include <QBrush>
#include <QColor>
#include <QPixmap>

Coin::Coin(QGraphicsItem *parent) : QGraphicsEllipseItem(parent)
{
    setRect(0, 0, 48, 48);
    setPen(Qt::NoPen);
    m_coinType = 0;
}

void Coin::setCoinType(int type)
{
    m_coinType = type;
    QPixmap pix;

    switch(type) {
    case 0: pix = QPixmap("coin_GPT.png"); break;
    case 1: pix = QPixmap("coin_book.png"); break;
    case 2: pix = QPixmap("coin_email.png"); break;
    default: pix = QPixmap("coin_GPT.png"); break;
    }

    if(!pix.isNull()) {
        pix = pix.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        setBrush(QBrush(pix));
    } else {
        // 加载失败时显示黄色
        setBrush(QBrush(QColor(255, 255, 0)));
    }
}

Magnet::Magnet(QGraphicsItem *parent) : QGraphicsRectItem(parent)
{
    setRect(0, 0, 48, 48);
    setPen(Qt::NoPen);

    QPixmap pix("coin_coffee.png");
    if(!pix.isNull()) {
        pix = pix.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        setBrush(QBrush(pix));
    } else {
        setBrush(QBrush(QColor(50, 150, 255))); // 加载失败回退蓝色
    }
}