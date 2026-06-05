#include "ball.h"
#include <QBrush>
#include <QColor>

Ball::Ball()
{
    setRect(0, 0, 30, 30);
    setBrush(QColor(255, 50, 50));
    setPen(Qt::NoPen);

    m_vx = 0;
    m_vy = 0;
    m_hp = 3;
}

void Ball::moveLeft()
{
    m_vx = -10;
}

void Ball::moveRight()
{
    m_vx = 10;
}

void Ball::stopMove()
{
    m_vx = 0;
}

// ==============================
// 核心：匀减速上升 + 匀速下落
// ==============================
void Ball::updatePhysics()
{
    // 水平移动
    moveBy(m_vx, 0);

    moveBy(0, m_vy);
    m_vy += 0.25;

    // 边界限制
    if (x() < 0)
        setX(0);
    if (x() > 1000 - boundingRect().width())
        setX(1000 - boundingRect().width());
}

// 跳跃：初始向上速度（刚好跳150高度）
void Ball::jump()
{
    m_vy = -10; // 匀减速跳 150 像素高度
}

void Ball::decreaseHp()
{
    m_hp--;
}

int Ball::getHp() const
{
    return m_hp;
}

void Ball::resetVelocity()
{
    m_vx = 0;
    m_vy = 0;
}

qreal Ball::vy() const
{
    return m_vy;
}