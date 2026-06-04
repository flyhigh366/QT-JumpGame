#include "coin.h"
#include <QBrush>
#include <QColor>

Coin::Coin()
{
    // 设置金币为一个20x20像素的圆形
    setRect(0, 0, 20, 20);

    // 设置填充色为金色
    setBrush(QBrush(QColor(241, 196, 15)));

    // 设置边框为1像素宽的黑色
    setPen(QPen(Qt::black, 1));
}

