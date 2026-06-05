#include "ball.h"
#include <QBrush>
#include <QColor>
#include <QGraphicsScene> // 用于获取场景边界

Ball::Ball(QGraphicsItem *parent) : QGraphicsEllipseItem(parent)
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
    m_vx = -MOVE_SPEED;
}

void Ball::moveRight()
{
    m_vx = MOVE_SPEED;
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
    m_vy += GRAVITY;

    //限制最大下落速度
    if (m_vy > MAX_FALL_SPEED) {
        m_vy = MAX_FALL_SPEED;
    }

    // 边界限制
    qreal rightBound = 1000.0; // 如果加入了场景，可以用 scene()->width() 替代
    if (x() < 0) {
        setX(0);
    } else if (x() > rightBound - boundingRect().width()) {
        setX(rightBound - boundingRect().width());
    }
}

// 跳跃：初始向上速度（刚好跳150高度）
void Ball::jump(qreal bounceMultiplier)
{
    // 如果是双倍弹跳，bounceMultiplier 传 1.5 或 2.0
    m_vy = JUMP_VELOCITY * bounceMultiplier;
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
