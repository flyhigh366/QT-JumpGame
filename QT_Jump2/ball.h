#ifndef BALL_H
#define BALL_H

#include <QGraphicsEllipseItem>
#include<QPen>

class Ball : public QGraphicsEllipseItem
{
public:
    explicit Ball(QGraphicsItem *parent = nullptr);

    void moveLeft();
    void moveRight();
    void stopMove();
    void updatePhysics();
    void jump(qreal bounceMultiplier = 1.0);
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

    const qreal GRAVITY = 0.5;       // 重力加速度
    const qreal MAX_FALL_SPEED = 15.0; // 最大下落速度 (3 太慢了，建议调大)
    const qreal JUMP_VELOCITY = -14.0;// 跳跃初速度 (计算得出跳跃高度为 200)
    const qreal MOVE_SPEED = 10.0;    // 水平移动速度

    QGraphicsItem *parent = nullptr;
};

#endif // BALL_H
