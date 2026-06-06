#include "ball.h"
#include <QBrush>
#include <QColor>
#include <QPixmap>
#include <QGraphicsScene> // 用于获取场景边界
#include <QObject>

Ball::Ball(QGraphicsItem *parent) : QGraphicsPixmapItem(parent)
{
    m_isHurt = false;
    // 初始化受伤定时器
    m_hurtTimer = new QTimer();
    m_hurtTimer->setSingleShot(true); // 只触发一次

    // 使用 Lambda 表达式连接定时器，3秒后解除受伤状态
    QObject::connect(m_hurtTimer, &QTimer::timeout, [this]() {
        this->m_isHurt = false;
        this->updateAppearance(); // 状态改变，刷新外观
    });

    // 初始加载一次外观
    updateAppearance();
    m_vx = 0;
    m_vy = 0;
    m_hp = 3;

}
void Ball::setSkin(int skinType) {
    m_skinType = skinType;
    updateAppearance(); // 切换皮肤后立刻刷新外观
}

void Ball::updateAppearance()
{
    // 1. 根据当前皮肤编号，获取前缀
    QString prefix;
    switch(m_skinType) {
    case 1: prefix = "dog"; break;
    case 2: prefix = "cat"; break;
    case 3: prefix = "panda"; break;
    default: prefix = "ball"; break;
    }

    // 2. 根据小球当前状态，拼接图片名称
    QString imagePath;
    if (m_hp <= 0) {
        // 死了：力竭版
        imagePath = QString(":/new/prefix2/%1_dead.png").arg(prefix);
    }
    else if (m_isHurt) {
        // 刚扣血的无敌时间：受伤版
        imagePath = QString(":/new/prefix2/%1_nervous.png").arg(prefix);
    }
    else if (m_hp == 1) {
        // 精力只有1点：紧张版
        imagePath = QString(":/new/prefix2/%1_dead.png").arg(prefix);
    }
    else {
        // 安全/满血复活：正常版
        imagePath = QString(":/new/prefix2/%1_normal.png").arg(prefix);
    }

    QPixmap pixmap(imagePath);
    if (!pixmap.isNull()) {
        setPixmap(pixmap.scaled(60, 60, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    } else {
        qDebug() << "图片加载失败！检查路径:" << imagePath;
    }
}
void Ball::setHp(int hp) {
    m_hp = hp;
    updateAppearance(); // 顺便刷新一下外观
}
void Ball::moveLeft()
{
    m_vx = -MOVE_SPEED;
}

void Ball::moveRight()
{
    m_vx = MOVE_SPEED;
}

void Ball::stopMove()
{
    m_vx = 0;
}

// ==============================
// 核心：匀减速上升 + 匀速下落
// ==============================
void Ball::updatePhysics()
{
    // 水平移动
    moveBy(m_vx, 0);

    moveBy(0, m_vy);
    m_vy += GRAVITY;

    //限制最大下落速度
    if (m_vy > MAX_FALL_SPEED) {
        m_vy = MAX_FALL_SPEED;
    }

    // 边界限制
    qreal rightBound = 1000.0; // 如果加入了场景，可以用 scene()->width() 替代
    if (x() < 0) {
        setX(0);
    } else if (x() > rightBound - boundingRect().width()) {
        setX(rightBound - boundingRect().width());
    }
}

// 跳跃：初始向上速度（刚好跳150高度）
void Ball::jump(qreal bounceMultiplier)
{
    // 如果是双倍弹跳，bounceMultiplier 传 1.5 或 2.0
    m_vy = JUMP_VELOCITY * bounceMultiplier;
}

bool Ball::decreaseHp()
{
    // 如果已经处于受伤（无敌）状态，直接返回 false，表示本次没扣血
    if (m_isHurt) return false;

    m_hp--;

    if (m_hp > 0) {
        m_isHurt = true;           // 进入受伤状态
        m_hurtTimer->start(3000);  // 启动3秒 (3000毫秒) 倒计时
    }

    // 扣血后立刻刷新外观
    updateAppearance();

    return true; // 返回 true，表示扣血成功！
}

int Ball::getHp() const
{
    return m_hp;
}

void Ball::resetVelocity()
{
    m_vx = 0;
    m_vy = 0;
}

qreal Ball::vy() const
{
    return m_vy;
}
