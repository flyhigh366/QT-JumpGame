#ifndef PLATFORM_H
#define PLATFORM_H

#include <QGraphicsPixmapItem> // 替换原来的 QGraphicsRectItem 和 QBrush
#include <QPixmap>
class Platform : public QGraphicsPixmapItem
{
public:
    explicit Platform();
    virtual ~Platform() = default; //虚析构函数，防止内存泄漏

    // 多态接口
    virtual bool isSpike() const { return false; }
    virtual bool isSuper() const { return false; }   // 是否是双倍弹跳
    virtual bool isFragile() const { return false; } // 是否是易碎板
    virtual void resettype() {} // 默认空实现

    // 碎裂状态管理
    bool isBroken() const { return m_broken; }
    void setBroken() {
        m_broken = true;
        hide(); // Qt 内置方法，直接让平台在画面上消失
    }

    bool islast() const { return _is_last; }
    void setislast(bool is_or_no) { _is_last = is_or_no; }

    enum { Type = UserType + 1 };
    int type() const override { return Type; }

private:
    bool _is_last = false;
    bool m_broken = false;
};

class NormalPlatform : public Platform
{
public:
    explicit NormalPlatform();
    // isSpike() 默认返回 false，无需重写
};

class SpikePlatform : public Platform
{
public:
    explicit SpikePlatform();
    bool isSpike() const override { return m_isSpike; }
    void resettype() override;

private:
    bool m_isSpike = true;
};

// === 双倍弹跳平台 ===
class SuperPlatform : public Platform
{
public:
    explicit SuperPlatform();
    bool isSuper() const override { return true; }
};

// === 易碎平台 ===
class FragilePlatform : public Platform
{
public:
    explicit FragilePlatform();
    bool isFragile() const override { return true; }
};
#endif // PLATFORM_H
