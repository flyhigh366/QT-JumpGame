#ifndef MAGNET_H
#define MAGNET_H

#include <QGraphicsEllipseItem>
#include <QObject>
#include <QPen>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
class Magnet : public QGraphicsPixmapItem
{
public:
    explicit Magnet(QGraphicsItem *parent = nullptr);
    virtual ~Magnet() = default;

    enum { Type = UserType + 4 };
    int type() const override { return Type; }
};
#endif // MAGNET_H
