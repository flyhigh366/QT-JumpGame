#ifndef COIN_H
#define COIN_H
#include <QGraphicsEllipseItem>
#include <QObject>
#include <QPen>
class Coin: public QGraphicsEllipseItem
{
public:
    explicit Coin();

    enum {Type = UserType + 3};
    int type() const {return Type;}

signals:
};

#endif // COIN_H
