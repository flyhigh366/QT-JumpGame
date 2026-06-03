#include "gameview.h"
#include <QRandomGenerator>
#include <QMessageBox>
#include <QFont>
#include <QList>
#include <QGraphicsItem>
#include<QKeyEvent>
#include<QtGlobal>
#include<utility>

GameView::GameView(QWidget *parent)
    : QGraphicsView(parent)
{
    setFixedSize(GAME_W, GAME_H);
    setRenderHint(QPainter::Antialiasing);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_life = MAX_LIFE;
    m_score = 0;
    m_maxScore = 0;
    m_deathLineY = DEATH_LINE_INIT;
    m_gameOver = false;
    m_win = false;

    //滚动初始化
    m_scrollOffset = 0;
    m_cameraTopY = GAME_H * 0.35; //进入画面上35%开始滚屏

    labLife = new QLabel("生命：3", this);
    labLife->setGeometry(10,10,120,25);
    labLife->setFont(QFont("Arial",12));

    labScore = new QLabel("本局积分：0", this);
    labScore->setGeometry(10,35,120,25);
    labScore->setFont(QFont("Arial",12));

    labMaxScore = new QLabel("最高积分：0", this);
    labMaxScore->setGeometry(10,60,120,25);
    labMaxScore->setFont(QFont("Arial",12));

    //返回菜单按钮
    btnBack = new QPushButton("返回菜单",this);
    btnBack->setGeometry(GAME_W-110,10,95,28);
    btnBack->setStyleSheet("background:#666666;color:white;border-radius:4px;");
    connect(btnBack,&QPushButton::clicked,this,&GameView::backToMenu);

    initScene();
    m_timer = new QTimer(this);
    m_timer->setInterval(16);
    connect(m_timer, &QTimer::timeout, this, &GameView::gameLoop);
    m_timer->start();
}

void GameView::initScene()
{
    m_scene = new QGraphicsScene(0,0,GAME_W,GAME_H,this);
    setScene(m_scene);

    m_endLine = new EndLine(true);
    m_scene->addItem(m_endLine);

    m_deathLine = new EndLine(false);
    m_scene->addItem(m_deathLine);

    m_ball = new Ball();
    m_ball->setPos(GAME_W/2, GAME_H - 80);
    m_scene->addItem(m_ball);

    initPlatform();
    initCoin();
}

void GameView::initPlatform()
{
    Platform* base = new Platform(PLAT_NORMAL);
    base->setRect(GAME_W/2 - 80, GAME_H - 50, 160, 15);
    m_scene->addItem(base);

    for(int i = 1; i < 15; ++i)
    {
        int x = QRandomGenerator::global()->bounded(50, GAME_W-100);
        int y = GAME_H - 50 - i * 40;
        int type = QRandomGenerator::global()->bounded(0,4);
        Platform* plat = new Platform((PlatformType)type);
        plat->setRect(x, y, 70, 12);
        m_scene->addItem(plat);
    }
}

void GameView::initCoin()
{
    for(int i = 0; i < 8; ++i)
    {
        int x = QRandomGenerator::global()->bounded(30, GAME_W-30);
        int y = QRandomGenerator::global()->bounded(END_Y+20, DEATH_LINE_INIT-50);
        Coin* coin = new Coin();
        coin->setPos(x, y);
        m_scene->addItem(coin);
    }
}

void GameView::keyPressEvent(QKeyEvent *event)
{
    if(m_gameOver || m_win) return;
    if(event->key() == Qt::Key_Left)  m_ball->m_keyLeft = true;
    if(event->key() == Qt::Key_Right) m_ball->m_keyRight = true;
}
void GameView::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Left)  m_ball->m_keyLeft = false;
    if(event->key() == Qt::Key_Right) m_ball->m_keyRight = false;
}

void GameView::gameLoop()
{
    if(m_gameOver || m_win) return;
    const qreal gravity = 0.6;
    qreal vx = 0;
    if(m_ball->m_keyLeft)  vx = -BALL_SPEED_X;
    if(m_ball->m_keyRight) vx = BALL_SPEED_X;

    qreal vy = m_ball->getVy() + gravity;
    m_ball->setVelocity(vx, vy);
    m_ball->moveBy(vx, vy);

    //左右穿墙
    if(m_ball->x() < 0) m_ball->setX(GAME_W);
    if(m_ball->x() > GAME_W) m_ball->setX(0);

    //=====镜头滚动核心=====
    if(m_ball->y() < m_cameraTopY)
    {
        qreal scrollDis = m_cameraTopY - m_ball->y();
        m_scrollOffset += scrollDis;

        //所有平台金币下移
        QList<QGraphicsItem*> items = m_scene->items();
        for(QGraphicsItem* item : std::as_const(items))
        {
            if(qgraphicsitem_cast<Platform*>(item) || qgraphicsitem_cast<Coin*>(item))
                item->moveBy(0, scrollDis);
        }
        //死线同步下移
        m_deathLineY += scrollDis;
        m_deathLine->setLine(0, m_deathLineY, GAME_W, m_deathLineY);
        //小球固定画面位置
        m_ball->setY(m_cameraTopY);
        m_ball->setVy(0);
    }

    //获取全部平台，判断上方生成新板
    QList<Platform*> plats;
    for(auto it : m_scene->items())
        if(Platform* p = qgraphicsitem_cast<Platform*>(it)) plats<<p;

    qreal minPlatY = GAME_H;
    for(auto p : std::as_const(plats)) minPlatY = qMin(minPlatY, p->pos().y());

    //上方空白生成新平台
    if(minPlatY > -20)
    {
        int x = QRandomGenerator::global()->bounded(50, GAME_W-100);
        int type = QRandomGenerator::global()->bounded(0,4);
        int newY = minPlatY - QRandomGenerator::global()->bounded(35,60);
        Platform* newPlat = new Platform((PlatformType)type);
        newPlat->setRect(x, newY,70,12);
        m_scene->addItem(newPlat);
        //随机附带金币
        if(QRandomGenerator::global()->bounded(0,3)==0)
        {
            Coin* c = new Coin;
            c->setPos(x+35, newY-20);
            m_scene->addItem(c);
        }
    }

    //删除滑出屏幕下方的平台金币
    for(auto it : m_scene->items())
    {
        if((qgraphicsitem_cast<Platform*>(it)||qgraphicsitem_cast<Coin*>(it)) && it->y()>GAME_H+30)
        {
            m_scene->removeItem(it);
            delete it;
        }
    }

    //掉落死线扣血
    if(m_ball->y() > m_deathLineY)
    {
        m_life--;
        labLife->setText(QString("生命：%1").arg(m_life));
        if(m_life <= 0)
        {
            m_gameOver = true;
            QMessageBox::information(this,"游戏结束",QString("本局得分：%1\n最高：%2").arg(m_score).arg(m_maxScore));
            gameReset();
            return;
        }
        m_ball->setPos(GAME_W/2, GAME_H - 80);
        m_ball->setVelocity(0,0);
    }

    //到达终点通关
    if(m_ball->y() <= END_Y){ winGame(); return; }

    checkCollision();
}

void GameView::checkCollision()
{
    QList<QGraphicsItem*> collItems = m_ball->collidingItems();
    for(const auto &item : std::as_const(collItems))
    {
        if(Coin* coin = qgraphicsitem_cast<Coin*>(item))
        {
            m_scene->removeItem(coin);
            delete coin;
            m_score +=10;
            labScore->setText(QString("本局积分：%1").arg(m_score));
            continue;
        }
        if(Platform* plat = qgraphicsitem_cast<Platform*>(item))
        {
            if(m_ball->getVy()>0)
            {
                switch (plat->getType())
                {
                case PLAT_NORMAL: m_ball->setVy(BALL_JUMP_NORMAL); break;
                case PLAT_SPRING: m_ball->setVy(BALL_JUMP_SPRING); break;
                case PLAT_SPIKE:
                    m_life--;
                    labLife->setText(QString("生命：%1").arg(m_life));
                    plat->setSpikeDisable();
                    if(m_life<=0){
                        m_gameOver=true;
                        QMessageBox::information(this,"阵亡",QString("得分：%1").arg(m_score));
                        gameReset();
                        return;
                    }
                    m_ball->setVy(BALL_JUMP_NORMAL);
                    break;
                case PLAT_BROKEN:
                    if(!plat->isBroken()){
                        plat->setBroken();
                        m_scene->removeItem(plat); delete plat;
                        m_ball->setVy(BALL_JUMP_NORMAL);
                    }
                    break;
                }
            }
        }
    }
}

void GameView::winGame()
{
    m_win = true;
    if(m_score>m_maxScore) m_maxScore=m_score;
    labMaxScore->setText(QString("最高积分：%1").arg(m_maxScore));
    QMessageBox::information(this,"恭喜通关",QString("得分：%1\n历史最高：%2").arg(m_score).arg(m_maxScore));
    gameReset();
}

void GameView::gameReset()
{
    //清空旧平台金币
    QList<QGraphicsItem*> all = m_scene->items();
    for(auto it : std::as_const(all))
    {
        if(qgraphicsitem_cast<Platform*>(it)||qgraphicsitem_cast<Coin*>(it))
        {
            m_scene->removeItem(it); delete it;
        }
    }

    m_life = MAX_LIFE;
    m_score = 0;
    m_deathLineY = DEATH_LINE_INIT;
    m_scrollOffset = 0;
    m_gameOver = false;
    m_win = false;

    labLife->setText(QString("生命：%1").arg(m_life));
    labScore->setText("本局积分：0");
    m_deathLine->setLine(0,m_deathLineY,GAME_W,m_deathLineY);

    m_ball->setPos(GAME_W/2, GAME_H - 80);
    m_ball->setVelocity(0,0);

    initPlatform();
    initCoin();
}