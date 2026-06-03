#define GAME_W 800
#define END_Y 120
#define DEATH_LINE_INIT 600
#include "endline.h"
#include <QPen>


EndLine::EndLine(bool isEnd, QGraphicsItem *parent):QGraphicsLineItem(parent)
{
    if(isEnd){
        setLine(0, END_Y, GAME_W, END_Y);
        setPen(QPen(Qt::cyan,3));
    }else{
        setLine(0, DEATH_LINE_INIT, GAME_W, DEATH_LINE_INIT);
        setPen(QPen(Qt::darkRed,2));
    }
}