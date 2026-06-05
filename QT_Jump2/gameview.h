#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QSoundEffect>
#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QElapsedTimer>
#include <QKeyEvent>
#include <QGraphicsTextItem>

#include "ball.h"
#include "platform.h"
#include "coin.h"
namespace Ui {
class gameview;
}

class gameview : public QDialog
{
    Q_OBJECT

public:
    explicit gameview(QWidget *parent = nullptr);
    ~gameview();

signals:
    void backToMenu();

private slots:
    void gameUpdate();
    void spawnPlatform();
    void checkGameOver();
    void removeOutOfScreenItems();
    void magnetTick(); // 处理磁铁倒计时的槽函数

//磁铁
private:
    QList<Magnet*> m_magnets; // 保存场景里的磁铁道具
    // 磁吸状态相关
    bool m_isMagnetActive;
    int m_magnetTimeLeft;       // 剩余秒数
    QTimer *m_magnetTimer;      // 专属的倒计时器
    QGraphicsTextItem *m_magnetText; // UI 上显示磁铁倒计时

private:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void scrollScreen();
    void updateUI();
    void _init_platform_coin();
    void saveScore(); // 保存分数

    Ui::gameview *ui;

    // 游戏核心
    QGraphicsScene *m_scene;
    Ball *m_ball;
    QTimer *m_gameTimer;
    // QTimer *m_spawnTimer;
    QElapsedTimer m_gameTime;

    QList<Platform*> m_platforms;
    QList<Coin*> m_coins;
    Platform *m_lastSavePlatform;

    bool m_leftPress;
    bool m_rightPress;
    bool m_gameisover;
    const int SCREEN_W = 1000;
    const int SCREEN_H = 1000;
    const int DEAD_LINE = 0;

    int m_score;
    QGraphicsTextItem *m_scoreText;
    QGraphicsTextItem *m_hpText;
    QGraphicsTextItem *m_timeText;

private:
    QSoundEffect *m_coinSound; // 金币音效
    QSoundEffect *m_bgmSound; // 背景音乐

};

#endif // GAMEVIEW_H
