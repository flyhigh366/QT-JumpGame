#ifndef ENDLINE_H
#define ENDLINE_H
#include <QGraphicsLineItem>

class EndLine : public QGraphicsLineItem
{
public:
    explicit EndLine(bool isEnd, QGraphicsItem *parent = nullptr);
};
#endif // ENDLINE_H