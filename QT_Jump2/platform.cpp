#include "platform.h"

#include <QPen>

Platform::Platform(): QGraphicsPixmapItem(),_is_last(false)
{

}

NormalPlatform::NormalPlatform() : Platform()
{
    QPixmap pix(":/new/prefix2/plat_normal.png");
    setPixmap(pix.scaled(120, 20, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

// ==========================================
// 派生类 SpikePlatform 实现
// ==========================================
SpikePlatform::SpikePlatform() : Platform()
{
    // 尖刺平台图片
    QPixmap pix(":/new/prefix2/plat_spike.png");
    setPixmap(pix.scaled(120, 20, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void SpikePlatform::resettype(){
    m_isSpike = false;
    QPixmap pix(":/new/prefix2/plat_normal.png");
    setPixmap(pix.scaled(120, 20, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}


SuperPlatform::SuperPlatform() : Platform()
{
    QPixmap pix(":/new/prefix2/plat_super.png");
    setPixmap(pix.scaled(120, 20, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

FragilePlatform::FragilePlatform() : Platform()
{
    // 易碎板图片
    QPixmap pix(":/new/prefix2/plat_fragile.png");
    setPixmap(pix.scaled(120, 20, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}