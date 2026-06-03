#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include "ball.h"
#include "platform.h"
#include "coin.h"
#include "endline.h"

class GameView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GameView(QWidget *parent = nullptr);
signals:
    void backToMenu();

private:
    QGraphicsScene *m_scene;
    Ball *m_ball;
    EndLine *m_endLine;
    EndLine *m_deathLine;
    QTimer *m_timer;
    QPushButton* btnBack;

    //游戏数据
    int m_life;
    int m_score;
    int m_maxScore;
    qreal m_deathLineY;
    bool m_gameOver;
    bool m_win;

    //镜头滚动参数【新增】
    qreal m_scrollOffset;
    qreal m_cameraTopY;

    QLabel *labLife;
    QLabel *labScore;
    QLabel *labMaxScore;

    void initScene();
    void initPlatform();
    void initCoin();
    void checkCollision();
    void gameReset();
    void winGame();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
private slots:
    void gameLoop();
};
#endif // GAMEVIEW_H