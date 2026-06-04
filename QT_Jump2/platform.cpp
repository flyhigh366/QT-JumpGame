#include "platform.h"

#include <QPen>

Platform::Platform(): QGraphicsRectItem(),_is_last(false)
{
    setRect(0, 0, 120, 20);
    setPen(QPen(Qt::black));
}
bool Platform::islast(){

    return _is_last;
}
void Platform::setislast(bool is_or_no){
    _is_last=is_or_no;
    return;
}
void Platform::resettype(){
    return;
}
bool Platform::isSpike(){
    return false;
}


NormalPlatform::NormalPlatform():isspike(false)
{
    setBrush(QColor(50, 205, 50)); // green色
}
bool NormalPlatform::isSpike(){
    return isspike;
}
void NormalPlatform::resettype(){
    return;
}

SpikePlatform::SpikePlatform():isspike(true)
{
    setBrush(QColor(200, 80, 80)); // 红色
}
bool SpikePlatform::isSpike(){
    return isspike;
}
void SpikePlatform::resettype(){
    isspike=false;
    setBrush(QColor(80, 200, 80));
    return;
}