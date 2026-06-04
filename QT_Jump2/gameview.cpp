#include "gameview.h"
#include "ui_gameview.h"
#include<QList>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QKeyEvent>

gameview::gameview(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::gameview)
{
    ui->setupUi(this);
    setFixedSize(SCREEN_W, SCREEN_H);

    // ----------------------
    // 创建游戏场景
    // ----------------------
    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0, 0, SCREEN_W, SCREEN_H);

    // 让UI里的Dialog自动显示游戏画面
    QVBoxLayout *layout = new QVBoxLayout(ui->gameview_2);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    QGraphicsView *graphicsView = new QGraphicsView(m_scene, ui->gameview_2);
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    layout->addWidget(graphicsView);
    //change1
    graphicsView->setFocusPolicy(Qt::StrongFocus);
    graphicsView->setFocus();
    // 透明背景，显示你UI里的背景图
    graphicsView->setStyleSheet("background:transparent;");
    m_scene->setBackgroundBrush(Qt::transparent);

    // ----------------------
    // 小球
    // ----------------------
    m_ball = new Ball();
    m_ball->setPos(SCREEN_W/2 - 15, SCREEN_H - 100);
    m_scene->addItem(m_ball);

    // ----------------------
    // UI 文字
    // ----------------------
    m_score = 0;
    m_scoreText = new QGraphicsTextItem("得分: 0");
    m_scoreText->setDefaultTextColor(Qt::white);
    m_scoreText->setFont(QFont("Arial",16));
    m_scoreText->setPos(20,20);
    m_scene->addItem(m_scoreText);

    m_hpText = new QGraphicsTextItem("生命: 3");
    m_hpText->setDefaultTextColor(Qt::white);
    m_hpText->setFont(QFont("Arial",16));
    m_hpText->setPos(120,20);
    m_scene->addItem(m_hpText);

    m_timeText = new QGraphicsTextItem("时间: 30");
    m_timeText->setDefaultTextColor(Qt::white);
    m_timeText->setFont(QFont("Arial",16));
    m_timeText->setPos(220,20);
    m_scene->addItem(m_timeText);

    // ----------------------
    // 初始平台
    // ----------------------
    m_leftPress = false;
    m_rightPress = false;
    m_gameisover=false;
    m_lastSavePlatform = nullptr;

    Platform *startPlat = new NormalPlatform();
    startPlat->setPos(SCREEN_W/2 - 60, SCREEN_H - 50);
    m_platforms.append(startPlat);
    m_scene->addItem(startPlat);
    m_lastSavePlatform = startPlat;

    _init_platform_coin();

    // ----------------------
    // 定时器
    // ----------------------
    m_gameTimer = new QTimer(this);
    connect(m_gameTimer, &QTimer::timeout, this, &gameview::gameUpdate);
    m_gameTimer->start(16);

   // m_spawnTimer = new QTimer(this);
   // connect(m_spawnTimer, &QTimer::timeout, this, &gameview::spawnPlatform);
   // m_spawnTimer->start(800);

    m_gameTime.start();
    setFocusPolicy(Qt::StrongFocus);
    setFocus();
    // 确保事件能正确传递
    ui->gameview_2->setFocusPolicy(Qt::NoFocus);

}

gameview::~gameview()
{
    qDeleteAll(m_platforms);m_platforms.clear();
    qDeleteAll(m_coins);
    m_coins.clear();
    delete m_ball;
    delete ui;
    ui = nullptr;
}
void gameview::keyPressEvent(QKeyEvent *event)
{
    if(m_gameisover){
        return;
    }
    qDebug() << "Key pressed:" << event->key();
    if(event->key() == Qt::Key_Left)  m_leftPress = true;
    if(event->key() == Qt::Key_Right) m_rightPress = true;

}

void gameview::keyReleaseEvent(QKeyEvent *event)
{

    if(event->key() == Qt::Key_Left)  m_leftPress = false;
    if(event->key() == Qt::Key_Right) m_rightPress = false;

}
void gameview::_init_platform_coin(){
    //
    const qreal L = 60;
    const qreal M = SCREEN_W / 2 - 60;
    const qreal R = SCREEN_W - 120;

    QList<int> yList = {50, 150, 250, 350, 450,550,650,750,850};

    for (int y : yList)
    {
        // 每行重置可用位置
        QList<qreal> availableX = {L, M, R};

        // 每行 1~3 个
        int count = QRandomGenerator::global()->bounded(1, 4);

        for (int i = 0; i < count; ++i)
        {
            // 关键：随机范围 = 当前剩下的数量
            int randIdx = QRandomGenerator::global()->bounded(availableX.size());

            // 拿走并删除，不会重复
            qreal px = availableX.takeAt(randIdx);

            // 生成平台
            Platform *p;
            if (QRandomGenerator::global()->generateDouble() < 0.3)
                p = new SpikePlatform();
            else
                p = new NormalPlatform();

            p->setPos(px, y);
            m_platforms.append(p);
            m_scene->addItem(p);

            // 金币
            if (QRandomGenerator::global()->generateDouble() < 0.5)
            {
                Coin *c = new Coin();
                c->setPos(px + 50, y - 30);
                m_coins.append(c);
                m_scene->addItem(c);
            }
        }
    }
    //
    return;
}
// ----------------------
// 游戏主循环
// ----------------------
void gameview::gameUpdate()
{
  //debug  if (!m_ball) {
  //  qDebug() << "警告：m_ball 为空指针！";
 //return;  // 直接返回，避免崩溃
 //}
    // 时间
    int remain = qMax(0, 30 - (int)(m_gameTime.elapsed()/1000));
    m_timeText->setPlainText(QString("时间: %1").arg(remain));
    if(remain <= 0){
        m_gameTimer->stop();
        m_gameTime.invalidate();
        m_gameisover=true;
        QMessageBox::information(this,"成功","时间结束！");
        emit backToMenu();
        return;
    }

    // 移动
    if(m_leftPress) m_ball->moveLeft();
    else if(m_rightPress) m_ball->moveRight();
    else m_ball->stopMove();

    m_ball->updatePhysics();

    // 平台碰撞
    m_platforms.detach();
    for(Platform *p : m_platforms){
        if(m_ball->collidesWithItem(p) && m_ball->vy() >= 0 && m_ball->y() < p->y()+10){
            m_ball->jump();
            if(m_lastSavePlatform){
                m_lastSavePlatform->setislast(false);
            }

            if(p->isSpike()){
                m_ball->decreaseHp();
                p->resettype();
                checkGameOver();
                m_lastSavePlatform = p;
                m_lastSavePlatform->setislast(true);
            }else{
                m_lastSavePlatform = p;
                m_lastSavePlatform->setislast(true);
            }
        }
    }
    // 吃金币 - 另一种安全写法
    QList<Coin*> coinsToDelete;
    // 在 gameUpdate 开头或者处理金币前执行
    m_coins.detach(); // 强制脱离共享，确保没人能在我遍历时捣乱
    for(Coin *c : m_coins){
        if(m_ball->collidesWithItem(c)){
            m_score += 10;
            m_scene->removeItem(c);
            coinsToDelete.append(c);
        }
    }
    // 统一删除
    for(Coin *c : coinsToDelete){
        m_coins.removeOne(c);
        delete c;
    }

    // 掉落死线
    if(m_ball->y() > DEAD_LINE + SCREEN_H + 100){
        m_ball->decreaseHp();
        checkGameOver();

        if(m_lastSavePlatform&&m_ball){
            if(m_ball->getHp()>0){
            m_ball->setPos(m_lastSavePlatform->x()+60 -15, m_lastSavePlatform->y()-30);
            m_ball->resetVelocity();
            }
        }

    }
    if(m_ball&&m_ball->y() < SCREEN_H/2){
        scrollScreen();
        spawnPlatform();
        removeOutOfScreenItems();
    }

    updateUI();
}

// ----------------------
// 生成平台
// ----------------------
void gameview::spawnPlatform()
{
    const qreal L = 60;
    const qreal M1 = SCREEN_W / 3 - 60;
    const qreal M2 = (SCREEN_W /3)*2- 60;
    const qreal R=SCREEN_W - 60;
    QList<int> yList = {50, 150, 250, 350, 450};

    for (int y : yList)
    {
        // 每行重置可用位置
        QList<qreal> availableX = {L, M1,M2, R};

        // 每行 1~4个
        int count = QRandomGenerator::global()->bounded(1, 5);

        for (int i = 0; i < count; ++i)
        {
            // 关键：随机范围 = 当前剩下的数量
            int randIdx = QRandomGenerator::global()->bounded(availableX.size());

            // 拿走并删除，不会重复
            qreal px = availableX.takeAt(randIdx);

            // 生成平台
            Platform *p;
            if (QRandomGenerator::global()->generateDouble() < 0.3)
                p = new SpikePlatform();
            else
                p = new NormalPlatform();

            p->setPos(px, y);
            m_platforms.append(p);
            m_scene->addItem(p);

            // 金币
            if (QRandomGenerator::global()->generateDouble() < 0.5)
            {
                Coin *c = new Coin();
                c->setPos(px + 50, y - 30);
                m_coins.append(c);
                m_scene->addItem(c);
            }
        }
    }
}

void gameview::scrollScreen()
{
    if(m_ball->y() < SCREEN_H/2){
        qreal dy = SCREEN_H/2;
        m_platforms.detach();
        for(Platform *p : m_platforms){
            if(p)p->moveBy(0, dy);
        }
        m_coins.detach();
        for(Coin *c : m_coins) {
            if(c) c->moveBy(0, dy);
        }
        m_ball->moveBy(0, dy);
    }
}
// ----------------------
// 移除屏幕外元素
// ----------------------
void gameview::removeOutOfScreenItems()
{
    // ========== 安全删除超出屏幕的平台 ==========
    QList<Platform*> delPlat;
    m_platforms.detach();
    for(auto p : m_platforms)
        if(p->y() > SCREEN_H)
            delPlat.append(p);

    for(auto p : delPlat) {
        if(p->islast()){
            if(m_lastSavePlatform){
                m_lastSavePlatform=nullptr;
            }
        }
        m_platforms.removeOne(p);
        m_scene->removeItem(p);
        delete p;
    }

    // ========== 安全删除超出屏幕的金币 ==========
    QList<Coin*> delCoin;
    m_coins.detach();
    for (Coin* c : m_coins)
        if (c->y() > SCREEN_H)
            delCoin << c;
    m_coins.detach();
    for (Coin* c : delCoin) {
        m_scene->removeItem(c);
        m_coins.removeOne(c);
        delete c;
    }
}

// ----------------------
// 游戏结束
// ----------------------
void gameview::checkGameOver()
{
    if(m_ball->getHp() <= 0){
        m_gameTimer->stop();
        m_gameTime.invalidate();
        m_gameisover=true;
        QMessageBox::warning(this,"游戏结束","生命值耗尽");
        emit backToMenu();
        this->close();
    }
}

// ----------------------
// 更新UI
// ----------------------
void gameview::updateUI()
{
    m_scoreText->setPlainText(QString("得分: %1").arg(m_score));
    m_hpText->setPlainText(QString("生命: %1").arg(m_ball->getHp()));
}