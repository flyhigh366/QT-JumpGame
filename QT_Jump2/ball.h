#ifndef BALL_H
#define BALL_H
// 1. 把 QGraphicsEllipseItem 换成 QGraphicsPixmapItem
#include <QGraphicsPixmapItem>
#include<QPen>
#include <QTimer>

class Ball : public QGraphicsPixmapItem
{
    // 在 Ball 类中增加：
public:
    void setSkin(int skinType); // 0=默认, 1=狗, 2=猫, 3=熊猫

private:
    int m_skinType = 0;
public:
    explicit Ball(QGraphicsItem *parent = nullptr);

    void moveLeft();
    void moveRight();
    void stopMove();
    void updatePhysics();
    void jump(qreal bounceMultiplier = 1.0);
    bool decreaseHp();
    int getHp() const;
    void resetVelocity();
    void setHp(int hp);

    qreal vy() const;

    enum { Type = UserType + 2 };
    int type() const override { return Type; }

private:
    qreal m_vx;
    qreal m_vy;
    int m_hp;

    bool m_isHurt;
    void updateAppearance(); // 核心：专门用来刷新小球外观的方法
    QTimer *m_hurtTimer;

    const qreal GRAVITY = 0.5;       // 重力加速度
    const qreal MAX_FALL_SPEED = 15.0; // 最大下落速度 (3 太慢了，建议调大)
    const qreal JUMP_VELOCITY = -14.0;// 跳跃初速度 (计算得出跳跃高度为 200)
    const qreal MOVE_SPEED = 10.0;
    // 水平移动速度

    QGraphicsItem *parent = nullptr;
};

#endif // BALL_H
