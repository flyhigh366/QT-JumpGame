#include "coin.h"
#include<QBrush>
Coin::Coin(QGraphicsItem *parent):QGraphicsEllipseItem(parent)
{
    setRect(-8, -8, 16, 16);
    setBrush(QBrush(Qt::yellow));
}