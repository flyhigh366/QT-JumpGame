#define BALL_RADIUS 10 // 自定义数值，例如10像素
#include "ball.h"
#include<QBrush>

Ball::Ball(QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent),m_keyLeft(false),  m_keyRight(false), m_vx(0), m_vy(0)
{
    setRect(-BALL_RADIUS, -BALL_RADIUS, BALL_RADIUS*2, BALL_RADIUS*2);
    setBrush(QBrush(Qt::blue));
}
void Ball::setVelocity(qreal vx, qreal vy){ m_vx=vx; m_vy=vy; }
qreal Ball::getVx() const{return m_vx;}
qreal Ball::getVy() const{return m_vy;}
void Ball::setVy(qreal y){m_vy=y;}