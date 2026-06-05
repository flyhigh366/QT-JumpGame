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
    graphicsView->setFocusPolicy(Qt::NoFocus);
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
    // 金币音效
    // ----------------------
    m_coinSound = new QSoundEffect(this);
    // 这里假设你把 ding.wav 放到了与工程同目录的 sounds 文件夹下
    // 最规范的做法是使用 Qt 资源文件 (qrc)，路径写 "qrc:/sounds/ding.wav"
    // 测试时你可以先填入你电脑上的绝对路径，例如 "C:/MyGame/ding.wav"
    m_coinSound->setSource(QUrl("qrc:/sounds/coin1.wav"));
    m_coinSound->setVolume(1.0f); // 音量拉满

    // ----------------------
    // 背景音乐
    // ----------------------
    m_bgmSound = new QSoundEffect(this);
    // 设置音乐路径
    m_bgmSound->setSource(QUrl("qrc:/sounds/song1.wav"));
    // 背景音乐建议音量调小，以免盖过吃金币的“叮”声
    m_bgmSound->setVolume(0.3f);
    // 无限循环
    m_bgmSound->setLoopCount(QSoundEffect::Infinite);
    // 游戏一开始就直接播放
    m_bgmSound->play();


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

    m_magnetText = new QGraphicsTextItem(""); // 默认是空的，不显示
    m_magnetText->setDefaultTextColor(Qt::cyan); // 青色字
    m_magnetText->setFont(QFont("Arial", 16, QFont::Bold));
    m_magnetText->setPos(320, 20);
    m_scene->addItem(m_magnetText);

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

    // 初始化磁铁状态
    m_isMagnetActive = false;
    m_magnetTimeLeft = 0;
    m_magnetTimer = new QTimer(this);
    connect(m_magnetTimer, &QTimer::timeout, this, &gameview::magnetTick);

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
    //
    const qreal L = 60;
    const qreal M = SCREEN_W / 2 - 60;
    const qreal R = SCREEN_W - 120;

    QList<int> yList = {150,300,450,600,750,900};

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

            // 金币与磁铁
            double itemRand = QRandomGenerator::global()->generateDouble();
            if (itemRand < 0.005) {
                // 0.5% 概率生成磁铁
                Magnet *m = new Magnet();
                m->setPos(px + 50, y - 30);
                m_magnets.append(m);
                m_scene->addItem(m);
            }
            else if (itemRand < 0.50) {
                // 45% 概率生成金币
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
    int remain = qMax(0, 150 - (int)(m_gameTime.elapsed()/1000));
    m_timeText->setPlainText(QString("时间: %1").arg(remain));
    if(remain <= 0){
        m_gameTimer->stop();
        m_gameTime.invalidate();
        m_gameisover=true;
        if (m_bgmSound) {
            m_bgmSound->stop(); // 游戏结束，停止播放背景音乐
        }
        QMessageBox::information(this,"成功","时间结束！");
        saveScore();
        emit backToMenu();
        this->close();
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
        // 如果板子已经碎了，就不再进行碰撞判定
        if (p->isBroken()) continue;

        if(m_ball->collidesWithItem(p) && m_ball->vy() >= 0 && m_ball->y() < p->y()+10){

            // 1. 判断弹跳力度
            if (p->isSuper()) {
                m_ball->jump(1.5); // 踩到弹簧板，1.5倍高度 (可以自己调)
            } else {
                m_ball->jump(1.0); // 普通高度
            }

            // 2. 取消旧存档点
            if(m_lastSavePlatform){
                m_lastSavePlatform->setislast(false);
            }

            // 3. 处理陷阱与碎裂机制
            if(p->isSpike()){
                m_ball->decreaseHp();
                p->resettype();
                checkGameOver();
                m_lastSavePlatform = p;
                m_lastSavePlatform->setislast(true);
            }
            else if (p->isFragile()) {
                // 踩到易碎板：只碎裂,不作为安全存档点
                p->setBroken();
            }
            else {
                // 普通板和弹簧板可以作为安全存档点
                m_lastSavePlatform = p;
                m_lastSavePlatform->setislast(true);
            }
        }
    }


    //金币与磁铁
    // ==========================================
    // 1. 吃到磁铁道具
    // ==========================================
    QList<Magnet*> magnetsToDelete;
    for (Magnet *m : m_magnets) {
        if (m_ball->collidesWithItem(m)) {
            // 吃到磁铁，激活状态！
            m_isMagnetActive = true;
            m_magnetTimeLeft = 5; // 设定 5 秒
            m_magnetText->setPlainText(QString("磁铁: 5s"));
            m_magnetTimer->start(1000); // 启动 1 秒 1 跳的倒计时

            m_scene->removeItem(m);
            magnetsToDelete.append(m);
        }
    }
    for (Magnet *m : magnetsToDelete) {
        m_magnets.removeOne(m);
        delete m;
    }

    // ==========================================
    // 2. 处理金币 (只有在磁吸激活时，才把距离拉过来)
    // ==========================================
    QList<Coin*> coinsToDelete;
    qreal ballCenterX = m_ball->x() + m_ball->boundingRect().width() / 2;
    qreal ballCenterY = m_ball->y() + m_ball->boundingRect().height() / 2;

    for (Coin *c : m_coins) {
        // --- 磁吸动画逻辑 (加了限制条件：必须激活磁吸状态) ---
        if (m_isMagnetActive) {
            const qreal MAGNET_RADIUS = 250.0; // 范围可以大一点
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

        // --- 碰撞吃掉逻辑 ---
        if (m_ball->collidesWithItem(c)) {
            m_score += 10;
            if (m_coinSound) m_coinSound->play();
            m_scene->removeItem(c);
            coinsToDelete.append(c);
        }
    }
    for (Coin *c : coinsToDelete) {
        m_coins.removeOne(c);
        delete c;
    }

    // 掉落死线
    if(m_ball->y() > DEAD_LINE + SCREEN_H + 100){
        m_ball->decreaseHp();
        checkGameOver();

        // 如果游戏还没结束，强制把小球拉回安全位置
        if(m_ball && m_ball->getHp() > 0){
            if(m_lastSavePlatform){
                // 有记录的板子，传送到板子正上方 (加上偏移量防止卡在板子边缘)
                m_ball->setPos(m_lastSavePlatform->x() + 45, m_lastSavePlatform->y() - 30);
            } else {
                // 如果没有板子记录，传送到屏幕中间的半空重试
                m_ball->setPos(SCREEN_W / 2 - 15, 100);
            }
            m_ball->resetVelocity();

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
// 生成平台 (优化版：基于距离的动态生成)
// ----------------------
void gameview::spawnPlatform()
{
    // 1. 寻找当前屏幕上位置最高（Y值最小）的平台
    qreal minY = SCREEN_H;
    for (Platform *p : m_platforms) {
        if (p->y() < minY) {
            minY = p->y();
        }
    }

    // 2. 只有当最上面的平台掉到距离顶部超过 150 像素时，才在它上面生成新的一行
    if (minY > 150) {
        const qreal L = 60;
        const qreal M1 = SCREEN_W / 3 - 60;
        const qreal M2 = (SCREEN_W / 3) * 2 - 60;
        const qreal R = SCREEN_W - 180;

        QList<qreal> availableX = {L, M1, M2, R};

        // 每次只生成 1~3 个，防止满屏都是板子
        int count = QRandomGenerator::global()->bounded(1, 4);

        for (int i = 0; i < count; ++i)
        {
            int randIdx = QRandomGenerator::global()->bounded(availableX.size());
            qreal px = availableX.takeAt(randIdx);

            // 生成平台
            Platform *p;
            double randVal = QRandomGenerator::global()->generateDouble();
            if (randVal < 0.20) {
                p = new SpikePlatform();   // 20% 概率是尖刺
            } else if (randVal < 0.35) {
                p = new SuperPlatform();   // 15% 概率是弹簧板
            } else if (randVal < 0.50) {
                p = new FragilePlatform(); // 15% 概率是易碎板
            } else {
                p = new NormalPlatform();  // 50% 概率是普通板
            }

            // 新板子永远生成在最高板子的正上方 150 像素处
            p->setPos(px, minY - 150);
            m_platforms.append(p);
            m_scene->addItem(p);

            // 金币与磁铁
            double itemRand = QRandomGenerator::global()->generateDouble();
            if (itemRand < 0.005) {
                // 0.5% 概率生成磁铁
                Magnet *m = new Magnet();
                m->setPos(px + 50, minY - 180);
                m_magnets.append(m);
                m_scene->addItem(m);
            }
            else if (itemRand < 0.50) {
                // 45% 概率生成金币
                Coin *c = new Coin();
                c->setPos(px + 50, minY - 180);
                m_coins.append(c);
                m_scene->addItem(c);
            }
        }
    }
}

//实现倒计时
void gameview::magnetTick()
{
    m_magnetTimeLeft--;
    if (m_magnetTimeLeft > 0) {
        m_magnetText->setPlainText(QString("磁铁: %1s").arg(m_magnetTimeLeft));
    } else {
        // 时间到，关闭磁吸
        m_isMagnetActive = false;
        m_magnetTimer->stop();
        m_magnetText->setPlainText(""); // 隐藏文字
    }
}

void gameview::scrollScreen()
{
    // 当小球跳过屏幕一半时，所有物体整体下移
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
    QList<Platform*> delPlat;
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
        delete p; // 这里现在是安全的，因为有了虚析构函数
    }

    // ========== 安全删除超出屏幕的金币 ==========
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

    // ========== 安全删除超出屏幕的磁铁 ==========
    QList<Magnet*> delMagnet;
    for (Magnet* m : m_magnets) {
        if (m->y() > SCREEN_H) {
            delMagnet.append(m); // 收集掉出屏幕的磁铁
        }
    }

    for (Magnet* m : delMagnet) {
        m_scene->removeItem(m);  // 从画面中移除
        m_magnets.removeOne(m);  // 从列表中移除
        delete m;                // 彻底释放内存
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
        if (m_bgmSound) {
            m_bgmSound->stop(); // 游戏结束，停止播放背景音乐
        }
        QMessageBox::warning(this,"游戏结束","生命值耗尽");
        saveScore();
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

// ----------------------
// 保存分数到本地 txt 文件
// ----------------------
void gameview::saveScore()
{
    if (m_score <= 0) return; // 0分

    QString fileName = "scores.txt";
    QList<QPair<int, QString>> scoreList;

    // 1. 读取现有的所有记录
    QFile readFile(fileName);
    if (readFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&readFile);
        while (!in.atEnd()) {
            QString line = in.readLine();
            // 按照写入的格式分割，提取出分数
            QStringList parts = line.split(" - 得分: ");
            if (parts.size() == 2) {
                int s = parts[1].toInt();
                // 用 QPair 把分数和整行文本绑在一起存起来
                scoreList.append(qMakePair(s, line));
            }
        }
        readFile.close();
    }

    // 2. 加入当前这局的新分数
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
    QString newLine = currentTime + " - 得分: " + QString::number(m_score);
    scoreList.append(qMakePair(m_score, newLine));

    // 3. 核心：按分数从大到小排序 (Lambda 表达式写法)
    std::sort(scoreList.begin(), scoreList.end(), [](const QPair<int, QString> &a, const QPair<int, QString> &b) {
        return a.first > b.first; // 降序排列
    });

    // 4. 剔除多余数据，只保留前 10 名
    while (scoreList.size() > 10) {
        scoreList.removeLast();
    }

    // 5. 覆盖写入回文件 (Truncate 模式会清空原文件再写)
    QFile writeFile(fileName);
    if (writeFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QTextStream out(&writeFile);
        for (const auto &item : scoreList) {
            out << item.second << "\n";
        }
        writeFile.close();
    }
}
