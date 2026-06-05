#ifndef PLATFORM_H
#define PLATFORM_H


#include <QGraphicsRectItem>

class Platform: public QGraphicsRectItem
{
public:
    explicit  Platform();
    bool virtual isSpike();
    bool _is_last=false;
    bool islast();
    void setislast(bool is_or_no);
    void virtual  resettype();
    enum { Type = UserType + 1 };
    int type() const override { return Type; }
};
class NormalPlatform : public Platform
{
public:
    bool isSpike();
    bool isspike;
    void resettype();
    explicit NormalPlatform();
};

class SpikePlatform : public Platform
{
public:
    void resettype();
    bool isSpike();
    bool isspike;
    explicit SpikePlatform();
};
#endif // PLATFORM_H
