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

    // ===== 任务6：皮肤状态系统（新增） =====
    enum BallState {
        StateNormal,   // 正常 - 红色
        StateScared,   // 惊恐 - 黄色（踩到刺板 / DDL过近）
        StateCool      // 淡定 - 蓝色（可扩展）
    };

    void setBallState(BallState state);
    BallState ballState() const;
    // =====================================

private:
    qreal m_vx;
    qreal m_vy;
    int m_hp;

    BallState m_state;  // 当前状态（新增）

    const qreal GRAVITY = 0.5;       // 重力加速度
    const qreal MAX_FALL_SPEED = 15.0; // 最大下落速度
    const qreal JUMP_VELOCITY = -14.0;// 跳跃初速度
    const qreal MOVE_SPEED = 10.0;    // 水平移动速度

    QGraphicsItem *parent = nullptr;
};

#endif // BALL_H