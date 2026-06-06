#include "magnet.h"


Magnet::Magnet(QGraphicsItem *parent) : QGraphicsPixmapItem(parent)
{
    // 加载磁铁道具图片
    QPixmap pix(":/new/prefix2/coin_coffee.png");

    // 同样缩放到 40x40
    setPixmap(pix.scaled(40, 40, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}