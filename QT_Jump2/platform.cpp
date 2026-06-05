#include "platform.h"

#include <QPen>

Platform::Platform(): QGraphicsRectItem(),_is_last(false)
{
    setRect(0, 0, 120, 20);
    setPen(QPen(Qt::black));
}

NormalPlatform::NormalPlatform() : Platform()
{
    // 普通平台只需设置颜色即可，isSpike 默认返回 false（继承自基类）
    setBrush(QColor(50, 205, 50)); // 绿色
}

// ==========================================
// 派生类 SpikePlatform 实现
// ==========================================
SpikePlatform::SpikePlatform() : Platform()
{
    // 尖刺平台初始化颜色，m_isSpike 默认值已经在头文件中设为 true
    setBrush(QColor(200, 80, 80)); // 红色
}


void SpikePlatform::resettype(){
    m_isSpike=false;
    setBrush(QColor(80, 200, 80));
}


SuperPlatform::SuperPlatform() : Platform()
{
    setBrush(QColor(50, 50, 255)); // 弹簧板设置为蓝色
}

FragilePlatform::FragilePlatform() : Platform()
{
    setBrush(QColor(150, 150, 150)); // 易碎板设置为灰色
    // 可以给易碎板加个虚线边框，看起来更不结实
    QPen p(Qt::black);
    p.setStyle(Qt::DashLine);
    setPen(p);
}