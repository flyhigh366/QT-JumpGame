#ifndef COIN_H
#define COIN_H
#include <QGraphicsEllipseItem>
#include <QObject>
#include <QPen>
#include <QGraphicsItem>
#include <QGraphicsRectItem>

class Coin: public QGraphicsEllipseItem
{
public:
    explicit Coin(QGraphicsItem *parent = nullptr);

    // 设置金币类型：0=GPT, 1=高数书, 2=邮件
    void setCoinType(int type);

    enum {Type = UserType + 3};
    int type() const {return Type;}

    int coinType() const { return m_coinType; }

private:
    int m_coinType;
};

class Magnet : public QGraphicsRectItem
{
public:
    explicit Magnet(QGraphicsItem *parent = nullptr);

    enum { Type = UserType + 4 };
    int type() const override { return Type; }
};
#endif // COIN_H