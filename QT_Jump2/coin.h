#ifndef COIN_H
#define COIN_H
#include <QGraphicsEllipseItem>
#include <QObject>
#include <QPen>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
class Coin: public QGraphicsPixmapItem
{
public:
    enum CoinType {
        GPT,
        MATH_BOOK,
        EMAIL
    };
    explicit Coin(QGraphicsItem *parent = nullptr);

    enum {Type = UserType + 3};
    int type() const {return Type;}

    CoinType getCoinType() const { return m_coinType; }
private:
    CoinType m_coinType;

signals:
};


#endif // COIN_H
