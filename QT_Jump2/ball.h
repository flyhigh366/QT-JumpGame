#ifndef BALL_H
#define BALL_H

#include <QGraphicsEllipseItem>
#include<QPen>

class Ball : public QGraphicsEllipseItem
{
public:
    explicit Ball();

    void moveLeft();
    void moveRight();
    void stopMove();
    void updatePhysics();
    void jump();
    void decreaseHp();
    int getHp() const;
    void resetVelocity();

    qreal vy() const;

    enum { Type = UserType + 2 };
    int type() const override { return Type; }

private:
    qreal m_vx;
    qreal m_vy;
    int m_hp;

    // 上升总高度 150（匀减速到0）
    const int jumpHeight = 150;
    // 匀速下落速度
    const int fallSpeed = 3;
};

#endif // BALL_H