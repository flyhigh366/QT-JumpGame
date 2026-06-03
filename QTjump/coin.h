#ifndef COIN_H
#define COIN_H
#include <QGraphicsEllipseItem>

class Coin : public QGraphicsEllipseItem
{
public:
    explicit Coin(QGraphicsItem *parent = nullptr);
};
#endif // COIN_H