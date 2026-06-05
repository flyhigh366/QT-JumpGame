#include "coin.h"
#include <QBrush>
#include <QColor>

Coin::Coin(QGraphicsItem *parent)
{
    // 设置金币为一个20x20像素的圆形
    setRect(0, 0, 20, 20);

    // 设置填充色为金色
    setBrush(QBrush(QColor(241, 196, 15)));

    // 设置边框为1像素宽的黑色
    setPen(QPen(Qt::black, 1));
}

Magnet::Magnet(QGraphicsItem *parent) : QGraphicsRectItem(parent)
{
    // 设置磁铁为 20x20 的正方形
    setRect(0, 0, 20, 20);
    setBrush(QBrush(QColor(50, 150, 255))); // 科技蓝色
    setPen(QPen(Qt::white, 2));             // 白色边框
}

