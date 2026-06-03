#include "platform.h"
#include<QPen>
#include<QBrush>
Platform::Platform(PlatformType type, QGraphicsItem *parent)
    : QGraphicsRectItem(parent), m_type(type), m_spikeValid(true), m_broken(false)
{
    setPen(QPen(Qt::black));
    switch (m_type)
    {
    case PLAT_NORMAL: setBrush(QBrush(Qt::lightGray)); break;
    case PLAT_SPIKE:  setBrush(QBrush(Qt::red)); break;
    case PLAT_SPRING: setBrush(QBrush(Qt::green)); break;
    case PLAT_BROKEN: setBrush(QBrush(Qt::darkGray)); break;
    }
}

PlatformType Platform::getType() const{ return m_type; }
void Platform::setSpikeDisable(){ m_spikeValid = false; setBrush(QBrush(Qt::lightGray)); }
bool Platform::isBroken() const{ return m_broken; }
void Platform::setBroken(){ m_broken = true; }