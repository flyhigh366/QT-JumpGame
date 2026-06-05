#include "ball.h"
#include <QBrush>
#include <QColor>
#include <QGraphicsScene>
#include <QPixmap>

Ball::Ball(QGraphicsItem *parent) : QGraphicsEllipseItem(parent)
{
    setRect(0, 0, 50, 50);   // 显示尺寸50x50
    setPen(Qt::NoPen);

    m_vx = 0;
    m_vy = 0;
    m_hp = 3;
    m_skinId = 0;            // 默认dog
    m_state = StateNormal;

    loadSkinImage();         // 加载初始图片
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

void Ball::increaseHp()
{
    const int MAX_HP = 3;
    if (m_hp < MAX_HP) {
        m_hp++;
    }
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

// ===== 皮肤系统实现 =====
void Ball::setSkin(int skinId)
{
    m_skinId = skinId;
    loadSkinImage();
}

void Ball::setState(BallState state)
{
    if (m_state == state) return;  // 状态没变就不重载
    m_state = state;
    loadSkinImage();
}

Ball::BallState Ball::currentState() const
{
    return m_state;
}

void Ball::loadSkinImage()
{
    QString skinName;
    switch(m_skinId) {
    case 0: skinName = "dog"; break;
    case 1: skinName = "cat"; break;
    case 2: skinName = "panda"; break;
    default: skinName = "dog"; break;
    }

    QString stateName;
    switch(m_state) {
    case StateNormal: stateName = "normal"; break;
    case StateNervous: stateName = "nervous"; break;
    case StateDead: stateName = "dead"; break;
    }

    QString fileName = QString("skin_%1_%2.png").arg(skinName).arg(stateName);
    QPixmap pix(fileName);

    if(!pix.isNull()) {
        pix = pix.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        setBrush(QBrush(pix));
    } else {
        // 加载失败回退颜色
        switch(m_state) {
        case StateNormal: setBrush(QBrush(QColor(255, 50, 50))); break;
        case StateNervous: setBrush(QBrush(QColor(255, 200, 50))); break;
        case StateDead: setBrush(QBrush(QColor(80, 80, 80))); break;
        }
    }
}