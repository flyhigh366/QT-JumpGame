#include "ball.h"
#include <QBrush>
#include <QColor>
#include <QGraphicsScene>

Ball::Ball(QGraphicsItem *parent) : QGraphicsEllipseItem(parent)
{
    setRect(0, 0, 30, 30);
    setBrush(QColor(255, 50, 50));
    setPen(Qt::NoPen);

    m_vx = 0;
    m_vy = 0;
    m_hp = 3;
    m_state = StateNormal;  // 初始状态：正常（新增）
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

void Ball::updatePhysics()
{
    moveBy(m_vx, 0);
    moveBy(0, m_vy);
    m_vy += GRAVITY;

    if (m_vy > MAX_FALL_SPEED) {
        m_vy = MAX_FALL_SPEED;
    }

    qreal rightBound = 1000.0;
    if (x() < 0) {
        setX(0);
    } else if (x() > rightBound - boundingRect().width()) {
        setX(rightBound - boundingRect().width());
    }
}

void Ball::jump(qreal bounceMultiplier)
{
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

// ===== 任务6：状态切换实现（新增） =====
void Ball::setBallState(BallState state)
{
    m_state = state;
    switch(state) {
    case StateNormal:
        setBrush(QColor(255, 50, 50));    // 红色 - 正常
        break;
    case StateScared:
        setBrush(QColor(255, 200, 50));  // 黄色 - 惊恐
        break;
    case StateCool:
        setBrush(QColor(50, 150, 255));  // 蓝色 - 淡定
        break;
    }
}

Ball::BallState Ball::ballState() const
{
    return m_state;
}
// =====================================