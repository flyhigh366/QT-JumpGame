#ifndef BALL_H
#define BALL_H
#include <QGraphicsEllipseItem>

class Ball : public QGraphicsEllipseItem
{
public:
    explicit Ball(QGraphicsItem *parent = nullptr);
    void setVelocity(qreal vx, qreal vy);
    qreal getVx() const;
    qreal getVy() const;
    void setVy(qreal y);

    bool m_keyLeft;
    bool m_keyRight;
private:
    qreal m_vx;
    qreal m_vy;
};
#endif // BALL_H