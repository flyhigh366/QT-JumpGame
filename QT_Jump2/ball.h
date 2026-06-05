#ifndef BALL_H
#define BALL_H

#include <QGraphicsEllipseItem>
#include <QPen>

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
    void increaseHp();
    int getHp() const;
    void resetVelocity();
    qreal vy() const;

    enum { Type = UserType + 2 };
    int type() const override { return Type; }

    // ===== 皮肤系统 =====
    enum BallState {
        StateNormal,   // 正常
        StateNervous,  // 惊恐（踩刺 / DDL逼近）
        StateDead      // 死亡（精力耗尽）
    };

    void setSkin(int skinId);      // 0=dog, 1=cat, 2=panda
    void setState(BallState state);
    BallState currentState() const;
    // ====================

private:
    void loadSkinImage();          // 内部加载图片

    qreal m_vx;
    qreal m_vy;
    int m_hp;
    int m_skinId;                  // 当前皮肤
    BallState m_state;             // 当前状态

    const qreal GRAVITY = 0.5;
    const qreal MAX_FALL_SPEED = 15.0;
    const qreal JUMP_VELOCITY = -14.0;
    const qreal MOVE_SPEED = 10.0;

    QGraphicsItem *parent = nullptr;
};

#endif // BALL_H