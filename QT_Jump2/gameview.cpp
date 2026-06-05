#include "gameview.h"
#include "ui_gameview.h"
#include<QList>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QKeyEvent>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <cmath>
#include <QPair>
#include <algorithm>
#include <QSettings>

gameview::gameview(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::gameview)
{
    ui->setupUi(this);
    setFixedSize(SCREEN_W, SCREEN_H);

    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0, 0, SCREEN_W, SCREEN_H);

    QVBoxLayout *layout = new QVBoxLayout(ui->gameview_2);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    QGraphicsView *graphicsView = new QGraphicsView(m_scene, ui->gameview_2);
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    layout->addWidget(graphicsView);
    graphicsView->setFocusPolicy(Qt::NoFocus);
    graphicsView->setStyleSheet("background:transparent;");
    m_scene->setBackgroundBrush(Qt::transparent);

    m_ball = new Ball();
    // 球尺寸改为50x50，半径25，居中偏移-25
    m_ball->setPos(SCREEN_W/2 - 25, SCREEN_H - 100);
    m_scene->addItem(m_ball);

    // 读取菜单保存的皮肤
    int savedSkin = QSettings().value("ballSkin", 0).toInt();
    m_ball->setSkin(savedSkin);

    m_coinSound = new QSoundEffect(this);
    m_coinSound->setSource(QUrl("qrc:/sounds/coin1.wav"));
    m_coinSound->setVolume(1.0f);

    m_bgmSound = new QSoundEffect(this);
    m_bgmSound->setSource(QUrl("qrc:/sounds/song1.wav"));
    m_bgmSound->setVolume(0.3f);
    m_bgmSound->setLoopCount(QSoundEffect::Infinite);
    m_bgmSound->play();

    m_score = 0;
    m_scoreText = new QGraphicsTextItem("灵感值: 0");
    m_scoreText->setDefaultTextColor(Qt::white);
    m_scoreText->setFont(QFont("Arial",16));
    m_scoreText->setPos(20,20);
    m_scene->addItem(m_scoreText);

    m_hpText = new QGraphicsTextItem("精力值: 3");
    m_hpText->setDefaultTextColor(Qt::white);
    m_hpText->setFont(QFont("Arial",16));
    m_hpText->setPos(120,20);
    m_scene->addItem(m_hpText);

    m_timeText = new QGraphicsTextItem("DDL倒计时: 30");
    m_timeText->setDefaultTextColor(Qt::white);
    m_timeText->setFont(QFont("Arial",16));
    m_timeText->setPos(220,20);
    m_scene->addItem(m_timeText);

    m_magnetText = new QGraphicsTextItem("");
    m_magnetText->setDefaultTextColor(Qt::cyan);
    m_magnetText->setFont(QFont("Arial", 16, QFont::Bold));
    m_magnetText->setPos(320, 20);
    m_scene->addItem(m_magnetText);

    m_leftPress = false;
    m_rightPress = false;
    m_gameisover=false;
    m_lastSavePlatform = nullptr;

    Platform *startPlat = new NormalPlatform();
    startPlat->setPos(SCREEN_W/2 - 60, SCREEN_H - 50);
    m_platforms.append(startPlat);
    m_scene->addItem(startPlat);
    m_lastSavePlatform = startPlat;

    m_isMagnetActive = false;
    m_magnetTimeLeft = 0;
    m_magnetTimer = new QTimer(this);
    connect(m_magnetTimer, &QTimer::timeout, this, &gameview::magnetTick);

    _init_platform_coin();

    m_gameTimer = new QTimer(this);
    connect(m_gameTimer, &QTimer::timeout, this, &gameview::gameUpdate);
    m_gameTimer->start(16);

    m_gameTime.start();
    m_totalTime = 150;
    setFocusPolicy(Qt::StrongFocus);
    setFocus();
    ui->gameview_2->setFocusPolicy(Qt::NoFocus);
}

gameview::~gameview()
{
    qDeleteAll(m_platforms);m_platforms.clear();
    qDeleteAll(m_coins);
    m_coins.clear();
    qDeleteAll(m_magnets);   m_magnets.clear();
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
    const qreal L = 60;
    const qreal M = SCREEN_W / 2 - 60;
    const qreal R = SCREEN_W - 120;

    QList<int> yList = {150,300,450,600,750,900};

    for (int y : yList)
    {

        QList< qreal > availableX = {L, M, R};
        int count = QRandomGenerator::global()->bounded(1, 4);

        for (int i = 0; i < count; ++i)
        {
            int randIdx = QRandomGenerator::global()->bounded(availableX.size());
            qreal px = availableX.takeAt(randIdx);

            Platform *p;
            if (QRandomGenerator::global()->generateDouble() < 0.3)
                p = new SpikePlatform();
            else
                p = new NormalPlatform();

            p->setPos(px, y);
            m_platforms.append(p);
            m_scene->addItem(p);

            double itemRand = QRandomGenerator::global()->generateDouble();
            if (itemRand < 0.005) {
                Magnet *m = new Magnet();
                m->setPos(px + 36, y - 50);
                m_magnets.append(m);
                m_scene->addItem(m);
            }
            else if (itemRand < 0.50) {
                Coin *c = new Coin();
                int coinRand = QRandomGenerator::global()->bounded(10);
                if(coinRand < 4) c->setCoinType(0);
                else if(coinRand < 7) c->setCoinType(1);
                else c->setCoinType(2);

                c->setPos(px + 36, y - 50);
                m_coins.append(c);
                m_scene->addItem(c);
            }
        }
    }
    return;
}

void gameview::gameUpdate()
{
    int remain = qMax(0, m_totalTime - (int)(m_gameTime.elapsed()/1000));
    if(remain <= 5 && remain > 0){
        m_timeText->setDefaultTextColor(Qt::red);
        m_timeText->setPlainText(QString("⚠️ DDL还有%1小时！").arg(remain));
    } else {
        m_timeText->setDefaultTextColor(Qt::white);
        m_timeText->setPlainText(QString("DDL倒计时: %1").arg(remain));
    }

    if(remain <= 0){
        m_gameTimer->stop();
        m_gameTime.invalidate();
        m_gameisover=true;
        if (m_bgmSound) {
            m_bgmSound->stop();
        }
        QMessageBox::information(this,"时间到","本局DDL时间已结束！");
        saveScore();
        emit backToMenu();
        this->close();
        return;
    }

    if(m_leftPress) m_ball->moveLeft();
    else if(m_rightPress) m_ball->moveRight();
    else m_ball->stopMove();

    m_ball->updatePhysics();

    // DDL追击警告
    qreal deadLineY = DEAD_LINE + SCREEN_H + 100;
    qreal distToDeadLine = deadLineY - m_ball->y();
    if (distToDeadLine < 150 && distToDeadLine > 0 && m_ball->getHp() > 0) {
        if (m_ball->currentState() != Ball::StateNervous) {
            m_ball->setState(Ball::StateNervous);
        }
    } else if (distToDeadLine >= 150) {
        if (m_ball->currentState() == Ball::StateNervous) {
            m_ball->setState(Ball::StateNormal);
        }
    }

    m_platforms.detach();
    for(Platform *p : m_platforms){
        if (p->isBroken()) continue;

        if(m_ball->collidesWithItem(p) && m_ball->vy() >= 0 && m_ball->y() < p->y()+10){

            if (p->isSuper()) {
                m_ball->jump(1.5);
            } else {
                m_ball->jump(1.0);
            }

            if(m_lastSavePlatform){
                m_lastSavePlatform->setislast(false);
            }

            if(p->isSpike()){
                m_ball->decreaseHp();
                m_ball->setState(Ball::StateNervous);
                p->resettype();
                checkGameOver();
                m_lastSavePlatform = p;
                m_lastSavePlatform->setislast(true);
            }
            else if (p->isFragile()) {
                p->setBroken();
            }
            else {
                m_lastSavePlatform = p;
                m_lastSavePlatform->setislast(true);
            }
        }
    }

    QList< Magnet* > magnetsToDelete;
    for (Magnet *m : m_magnets) {
        if (m_ball->collidesWithItem(m)) {
            m_isMagnetActive = true;
            m_ball->increaseHp();
            m_magnetTimeLeft = 5;
            m_magnetText->setPlainText(QString("灵感爆发: 5s"));
            m_magnetTimer->start(1000);

            m_scene->removeItem(m);
            magnetsToDelete.append(m);
        }
    }
    for (Magnet *m : magnetsToDelete) {
        m_magnets.removeOne(m);
        delete m;
    }

    QList<Coin*> coinsToDelete;
    qreal ballCenterX = m_ball->x() + m_ball->boundingRect().width() / 2;
    qreal ballCenterY = m_ball->y() + m_ball->boundingRect().height() / 2;

    for (Coin *c : m_coins) {
        if (m_isMagnetActive) {
            const qreal MAGNET_RADIUS = 250.0;
            const qreal MAGNET_SPEED = 15.0;

            qreal coinCenterX = c->x() + c->boundingRect().width() / 2;
            qreal coinCenterY = c->y() + c->boundingRect().height() / 2;

            qreal dx = ballCenterX - coinCenterX;
            qreal dy = ballCenterY - coinCenterY;
            qreal distance = std::hypot(dx, dy);

            if (distance < MAGNET_RADIUS && distance > 0) {
                c->moveBy((dx / distance) * MAGNET_SPEED, (dy / distance) * MAGNET_SPEED);
            }
        }

        if (m_ball->collidesWithItem(c)) {
            int type = c->coinType();

            if (type == 0) {
                m_score += 15;
            } else if (type == 1) {
                m_score += 10;
            } else if (type == 2) {
                m_score += 10;
                m_totalTime += 3;
            }

            if (m_coinSound) m_coinSound->play();
            m_scene->removeItem(c);
            coinsToDelete.append(c);
        }
    }
    for (Coin *c : coinsToDelete) {
        m_coins.removeOne(c);
        delete c;
    }

    if(m_ball->y() > DEAD_LINE + SCREEN_H + 100){
        m_ball->decreaseHp();
        checkGameOver();

        if(m_ball && m_ball->getHp() > 0){
            if(m_lastSavePlatform){
                // 球宽50，平台宽120，居中偏移(120-50)/2=35
                m_ball->setPos(m_lastSavePlatform->x() + 35, m_lastSavePlatform->y() - 30);
            } else {
                m_ball->setPos(SCREEN_W / 2 - 25, 100);
            }
            m_ball->resetVelocity();
            m_ball->setState(Ball::StateNormal);
        }
    }

    if(m_ball&&m_ball->y() < SCREEN_H/2){
        scrollScreen();
        spawnPlatform();
        removeOutOfScreenItems();
    }

    updateUI();
}

void gameview::spawnPlatform()
{
    qreal minY = SCREEN_H;
    for (Platform *p : m_platforms) {
        if (p->y() < minY) {
            minY = p->y();
        }
    }

    if (minY > 150) {
        const qreal L = 60;
        const qreal M1 = SCREEN_W / 3 - 60;
        const qreal M2 = (SCREEN_W / 3) * 2 - 60;
        const qreal R = SCREEN_W - 180;

        QList< qreal > availableX = {L, M1, M2, R};
        int count = QRandomGenerator::global()->bounded(1, 4);

        for (int i = 0; i < count; ++i)
        {
            int randIdx = QRandomGenerator::global()->bounded(availableX.size());
            qreal px = availableX.takeAt(randIdx);

            Platform *p;
            double randVal = QRandomGenerator::global()->generateDouble();
            if (randVal < 0.20) {
                p = new SpikePlatform();
            } else if (randVal < 0.35) {
                p = new SuperPlatform();
            } else if (randVal < 0.50) {
                p = new FragilePlatform();
            } else {
                p = new NormalPlatform();
            }

            p->setPos(px, minY - 150);
            m_platforms.append(p);
            m_scene->addItem(p);

            double itemRand = QRandomGenerator::global()->generateDouble();
            if (itemRand < 0.005) {
                Magnet *m = new Magnet();
                m->setPos(px + 36, minY - 200);
                m_magnets.append(m);
                m_scene->addItem(m);
            }
            else if (itemRand < 0.50) {
                Coin *c = new Coin();
                int coinRand = QRandomGenerator::global()->bounded(10);
                if(coinRand < 4) c->setCoinType(0);
                else if(coinRand < 7) c->setCoinType(1);
                else c->setCoinType(2);

                c->setPos(px + 36, minY - 200);
                m_coins.append(c);
                m_scene->addItem(c);
            }
        }
    }
}

void gameview::magnetTick()
{
    m_magnetTimeLeft--;
    if (m_magnetTimeLeft > 0) {
        m_magnetText->setPlainText(QString("灵感爆发: %1s").arg(m_magnetTimeLeft));
    } else {
        m_isMagnetActive = false;
        m_magnetTimer->stop();
        m_magnetText->setPlainText("");
    }
}

void gameview::scrollScreen()
{
    if(m_ball->y() < SCREEN_H/2){
        qreal dy = SCREEN_H/2 - m_ball->y();

        for(Platform *p : m_platforms){
            p->moveBy(0, dy);
        }
        for(Coin *c : m_coins) {
            c->moveBy(0, dy);
        }
        for(Magnet *m : m_magnets) {
            m->moveBy(0, dy);
        }

        m_ball->moveBy(0, dy);
    }
}

void gameview::removeOutOfScreenItems(){
    QList< Platform* > delPlat;
    for (Platform* p : m_platforms) {
        if (p->y() > SCREEN_H) {
            delPlat.append(p);
        }
    }

    for (Platform* p : delPlat) {
        if (p->islast() && m_lastSavePlatform == p) {
            m_lastSavePlatform = nullptr;
        }
        m_platforms.removeOne(p);
        m_scene->removeItem(p);
        delete p;
    }

    QList<Coin*> delCoin;
    for (Coin* c : m_coins) {
        if (c->y() > SCREEN_H) {
            delCoin.append(c);
        }
    }

    for (Coin* c : delCoin) {
        m_scene->removeItem(c);
        m_coins.removeOne(c);
        delete c;
    }

    QList< Magnet* > delMagnet;
    for (Magnet* m : m_magnets) {
        if (m->y() > SCREEN_H) {
            delMagnet.append(m);
        }
    }

    for (Magnet* m : delMagnet) {
        m_scene->removeItem(m);
        m_magnets.removeOne(m);
        delete m;
    }
}

void gameview::checkGameOver()
{
    if(m_ball->getHp() <= 0){
        m_gameTimer->stop();
        m_gameTime.invalidate();
        m_gameisover=true;
        if (m_bgmSound) {
            m_bgmSound->stop();
        }
        // 精力耗尽，切换死亡表情
        m_ball->setState(Ball::StateDead);
        QMessageBox::warning(this,"游戏结束","精力耗尽，被DDL吞噬了...");
        saveScore();
        emit backToMenu();
        this->close();
    }
}

void gameview::updateUI()
{
    m_scoreText->setPlainText(QString("灵感值: %1").arg(m_score));
    m_hpText->setPlainText(QString("精力值: %1").arg(m_ball->getHp()));
}

void gameview::saveScore()
{
    if (m_score <= 0) return;

    QString fileName = "scores.txt";
    QList<QPair<int, QString> > scoreList;

    QFile readFile(fileName);
    if (readFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&readFile);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(" - 灵感值: ");
            if (parts.size() == 2) {
                int s = parts[1].toInt();
                scoreList.append(qMakePair(s, line));
            }
        }
        readFile.close();
    }

    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
    QString newLine = currentTime + " - 灵感值: " + QString::number(m_score);
    scoreList.append(qMakePair(m_score, newLine));

    std::sort(scoreList.begin(), scoreList.end(), [](const QPair<int, QString> &a, const QPair<int, QString> &b) {
        return a.first > b.first;
    });

    while (scoreList.size() > 10) {
        scoreList.removeLast();
    }

    QFile writeFile(fileName);
    if (writeFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QTextStream out(&writeFile);
        for (const auto &item : scoreList) {
            out << item.second << "\n";
        }
        writeFile.close();
    }
}