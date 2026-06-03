#ifndef PLATFORM_H
#define PLATFORM_H

#include <QGraphicsRectItem>
#include <QBrush>

enum PlatformType
{
    PLAT_NORMAL,    //普通
    PLAT_SPIKE,     //尖刺扣血
    PLAT_SPRING,    //弹簧高跳
    PLAT_BROKEN     //踩碎消失
};

const int GAME_W = 800;
const int GAME_H = 600;
const int BALL_RADIUS = 15;
const int BALL_SPEED_X = 6;
const int BALL_JUMP_NORMAL = -18;
const int BALL_JUMP_SPRING = -54;
const int MAX_LIFE = 3;
const int END_Y = 80;
const int DEATH_LINE_INIT = 550;

class Platform : public QGraphicsRectItem
{
public:
    explicit Platform(PlatformType type, QGraphicsItem *parent = nullptr);
    PlatformType getType() const;
    void setSpikeDisable();
    bool isBroken() const;
    void setBroken();

private:
    PlatformType m_type;
    bool m_spikeValid;
    bool m_broken;
};

#endif // PLATFORM_H