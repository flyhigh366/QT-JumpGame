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

    enum {Type = UserType + 3};
    int type() const {return Type;}

signals:
};

class Magnet : public QGraphicsRectItem
{
public:
    explicit Magnet(QGraphicsItem *parent = nullptr);

    enum { Type = UserType + 4 };
    int type() const override { return Type; }
};
#endif // COIN_H
