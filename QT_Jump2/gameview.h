#ifndef GAMEVIEW_H
#define GAMEVIEW_H


#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QElapsedTimer>
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

private:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void scrollScreen();
    void removeOutOfScreenItems();
    void updateUI();
    void _init_platform_coin();

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
};

#endif // GAMEVIEW_H
