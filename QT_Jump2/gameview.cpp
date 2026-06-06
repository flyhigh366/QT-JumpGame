#include "gameview.h"
#include "ui_gameview.h"
#include "magnet.h"
#include<QList>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QKeyEvent>
#include <QFile>
#include <QSettings>
#include <QTextStream>
#include <QDateTime>
#include <QLabel>
#include <QPushButton>
#include <cmath>
#include <QPair>
#include <algorithm>
#include <QGraphicsDropShadowEffect>

gameview::gameview(QWidget *parent, int hp, int time,int skin)
    : QDialog(parent)
    , ui(new Ui::gameview), m_totalTime(time)
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
    m_ball->setSkin(skin);
    m_ball->setHp(hp);
    m_ball->setPos(SCREEN_W/2 - 15, SCREEN_H - 100);
    m_scene->addItem(m_ball);


    // ----------------------
    // 金币音效
    // ----------------------
    m_coinSound = new QSoundEffect(this);
    // 这里假设你把 ding.wav 放到了与工程同目录的 sounds 文件夹下
    // 最规范的做法是使用 Qt 资源文件 (qrc)，路径写 "qrc:/sounds/ding.wav"
    // 测试时你可以先填入你电脑上的绝对路径，例如 "C:/MyGame/ding.wav"
    m_coinSound->setSource(QUrl("qrc:/sounds/coin2.wav"));
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
    // 终局与成就音效
    // ----------------------
    m_winSound = new QSoundEffect(this);
    m_winSound->setSource(QUrl("qrc:/sounds/win1.wav")); // 准备一个欢呼或通关音效
    m_winSound->setVolume(1.0f);

    m_loseSound = new QSoundEffect(this);
    m_loseSound->setSource(QUrl("qrc:/sounds/gameover.wav")); // 准备一个悲惨的失败音效
    m_loseSound->setVolume(1.0f);

    m_achSound = new QSoundEffect(this);
    m_achSound->setSource(QUrl("qrc:/sounds/achievement1.wav")); // 准备一个“叮”的清脆解锁音效
    m_achSound->setVolume(1.0f);
    // ----------------------
    // 终局与成就音效
    // ----------------------
    m_winSound = new QSoundEffect(this);
    m_winSound->setSource(QUrl("qrc:/sounds/win.wav")); // 准备一个欢呼或通关音效
    m_winSound->setVolume(1.0f);

    m_loseSound = new QSoundEffect(this);
    m_loseSound->setSource(QUrl("qrc:/sounds/gameover.wav")); // 准备一个悲惨的失败音效
    m_loseSound->setVolume(1.0f);

    m_achSound = new QSoundEffect(this);
    m_achSound->setSource(QUrl("qrc:/sounds/achievement.wav")); // 准备一个“叮”的清脆解锁音效
    m_achSound->setVolume(1.0f);
    // ----------------------
    // 受伤音效
    // ----------------------
    m_hurtSound = new QSoundEffect(this);
    m_hurtSound->setSource(QUrl("qrc:/sounds/diaoxue.wav")); // 记得去网上找个短促的受伤/漏气音效，放进资源文件里
    m_hurtSound->setVolume(1.0f);


    // ----------------------
    // UI 文字
    // ----------------------
    m_score = 0;
    m_initialHp = hp;
    m_initialTime = time;
    // ... 其他代码不变
    m_scoreText = new QGraphicsTextItem("灵感值: 0");
    m_scoreText->setDefaultTextColor(Qt::white);
    m_scoreText->setFont(QFont("Arial",16));
    m_scoreText->setPos(20,20);
    m_scene->addItem(m_scoreText);
    // 构造函数中的 UI 文字部分，加上这三行
    m_magnetText = new QGraphicsTextItem(""); // 初始为空
    m_magnetText->setDefaultTextColor(Qt::yellow); // 灵感爆发用显眼的黄色
    m_magnetText->setFont(QFont("Arial",16, QFont::Bold));
    m_magnetText->setPos(350, 20); // 放在时间旁边
    m_scene->addItem(m_magnetText);


    // 修改 UI 文字初始显示
    m_hpText = new QGraphicsTextItem(QString("精力：%1").arg(hp)); // 动态显示初始生命
    m_hpText->setDefaultTextColor(Qt::white);
    m_hpText->setFont(QFont("Arial",16));
    m_hpText->setPos(120,20);
    m_scene->addItem(m_hpText);

    m_timeText = new QGraphicsTextItem(QString("时间: %1").arg(m_totalTime)); // 动态显示初始时间
    m_timeText->setDefaultTextColor(Qt::white);
    m_timeText->setFont(QFont("Arial",16));
    m_timeText->setPos(220,20);
    m_scene->addItem(m_timeText);

    // ----------------------
    // DDL 死线 (红色警告线)
    // ----------------------
    QPen ddlPen(QColor(255, 50, 50, 200)); // 设置为醒目的红色，带一点点透明度
    ddlPen.setWidth(6);                    // 粗细设为 6 像素
    ddlPen.setStyle(Qt::DashLine);         // 设为虚线，更有“警戒带”的氛围

    // 在屏幕最下方（y = SCREEN_H - 5）画一条横跨左右的线
    QGraphicsLineItem *ddlLine = new QGraphicsLineItem(0, SCREEN_H - 5, SCREEN_W, SCREEN_H - 5);
    ddlLine->setPen(ddlPen);
    ddlLine->setZValue(100);               // 设置很高的 Z 值，确保它压在木板和背景的上面，不会被遮
    m_scene->addItem(ddlLine);

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

    // --- 新增：按下 ESC 键触发暂停 ---
    if(event->key() == Qt::Key_Escape) {
        pauseGame();
        return; // 直接返回，不要再处理左右移动了
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
void gameview::_init_platform_coin()
{
    const qreal L = 60;
    const qreal M1 = SCREEN_W / 3 - 60;
    const qreal M2 = (SCREEN_W / 3) * 2 - 60;
    const qreal R = SCREEN_W - 180;
    QList<qreal> cols = {L, M1, M2, R}; // 把屏幕分为 4 列

    QList<int> yList = {750, 600, 450, 300, 150}; // 从下往上生成
    int lastCol = 1; // 开局主干道从 M1 (中左) 列开始

    for (int y : yList)
    {
        QList<qreal> availableX = cols;

        // 1. 生成【绝对安全】的主干道！
        // 下一层的保底平台只能在上一层的【左、中、右】三列中选一个，跨度绝不超过1列
        lastCol = lastCol + QRandomGenerator::global()->bounded(-1, 2);
        if (lastCol < 0) lastCol = 0; // 撞到左墙就停
        if (lastCol > 3) lastCol = 3; // 撞到右墙就停

        qreal safeX = cols[lastCol];
        availableX.removeOne(safeX); // 从备选位置中踢出这个安全位置

        // 放置保底的普通平台
        Platform *safeP = new NormalPlatform();
        safeP->setPos(safeX, y);
        m_platforms.append(safeP);
        m_scene->addItem(safeP);

        // 2. 在剩下的 3 个位置里，随机生成 0~2 个其他干扰平台
        int extraCount = QRandomGenerator::global()->bounded(0, 3);
        for (int i = 0; i < extraCount; ++i) {
            int randIdx = QRandomGenerator::global()->bounded(availableX.size());
            qreal px = availableX.takeAt(randIdx);

            Platform *p;
            if (QRandomGenerator::global()->generateDouble() < 0.3) p = new SpikePlatform();
            else p = new NormalPlatform();

            p->setPos(px, y);
            m_platforms.append(p);
            m_scene->addItem(p);

            // 开局随机给点金币
            if (QRandomGenerator::global()->generateDouble() < 0.3) {
                Coin *c = new Coin();
                c->setPos(px + 50, y - 30);
                m_coins.append(c);
                m_scene->addItem(c);
            }
        }
    }
}
// ----------------------
// 游戏主循环
// ----------------------
void gameview::gameUpdate()
{
    // 时间
    int remain = qMax(0, m_totalTime - (int)(m_gameTime.elapsed()/1000));

    if (remain <= 5 && remain > 0) {
        m_timeText->setPlainText(QString("⚠️ DDL还有%1秒！").arg(remain));
        m_timeText->setDefaultTextColor(Qt::red); // 文字变红
    } else {
        m_timeText->setPlainText(QString("时间: %1").arg(remain));
        m_timeText->setDefaultTextColor(Qt::white); // 正常情况下保持白色
    }
    if(remain <= 0){
        m_gameTimer->stop();
        m_gameTime.invalidate();
        m_gameisover=true;
        if (m_bgmSound) m_bgmSound->stop();

        // ==========================================
        // 挑战结局判定：根据灵感值决定生死
        // ==========================================
        if (m_score >= 150) {
            if (m_winSound) m_winSound->play();
            // 极简胜利文案
            QString winStory = QString("按时提交，代码跑通。\n\n最终灵感值: %1\n评级: A+ (Accepted)").arg(m_score);
            showStoryPopup("🎉挑战成功", winStory, 1);
        } else {
            if (m_loseSound) m_loseSound->play();
            // 极简失败文案
            QString loseStory = QString("DDL 已截止，提交失败。\n\n最终灵感值: %1\n评级: F (重修)").arg(m_score);
            showStoryPopup("💀挑战失败", loseStory, 2);
        }

      checkDeathAchievements("TIMEOUT");
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
                // 💡 只有当成功扣血时（不在无敌时间内），才播放音效和飘字！
                if (m_ball->decreaseHp()) {
                    if (m_hurtSound) m_hurtSound->play();
                    showDamageEffect(m_ball->x(), m_ball->y());
                }

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


    // ==========================================
    // 1. 吃到咖啡 (原磁铁道具)
    // ==========================================
    QList<Magnet*> magnetsToDelete;
    for (Magnet *m : m_magnets) {
        if (m_ball->collidesWithItem(m)) {
            m_isMagnetActive = true;
            m_magnetTimeLeft = 5;
            m_magnetText->setPlainText(QString("灵感爆发: 5s"));
            m_magnetTimer->start(1000);

            // --- 新增效果：回 1 精力 ---
            m_ball->setHp(m_ball->getHp() + 1);

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

            // 判断具体的金币类型并赋予不同效果
            if (c->getCoinType() == Coin::GPT) {
                m_score += 15; // GPT: +15 灵感
            }
            else if (c->getCoinType() == Coin::MATH_BOOK) {
                m_score += 10; // 高数书: +10 灵感
            }
            else if (c->getCoinType() == Coin::EMAIL) {
                m_score += 10; // 邮件: +10 灵感
                m_totalTime += 3; // 邮件: DDL 延后 3 秒 (增加总时间上限)
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

    // 掉落死线
    if(m_ball->y() > DEAD_LINE + SCREEN_H + 100){

        if (m_ball->decreaseHp()) {
            if (m_hurtSound) m_hurtSound->play();
            // 因为掉下去了，飘字的位置在屏幕底端会更合理
            showDamageEffect(m_ball->x(), SCREEN_H - 100);
        }

        if(m_ball->getHp() <= 0) {
            checkDeathAchievements("FALL"); // 触发掉落死亡
        }
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
    // 1. 获取当前屏幕最高 Y 值
    qreal minY = SCREEN_H;
    for (Platform *p : m_platforms) {
        if (p->y() < minY) {
            minY = p->y();
        }
    }

    // 只有当最顶部的平台距离画面顶端超过 150 像素时，才生成新的一层
    if (minY > 150) {
        const qreal L = 60;
        const qreal M1 = SCREEN_W / 3 - 60;
        const qreal M2 = (SCREEN_W / 3) * 2 - 60;
        const qreal R = SCREEN_W - 180;
        QList<qreal> cols = {L, M1, M2, R};

        // 2. 找到最高的一个“绝对安全”的平台 (普通板或弹簧板) 作为基准点
        qreal highestSafeX = M1;
        qreal highestSafeY = SCREEN_H;
        for (Platform *p : m_platforms) {
            if (!p->isSpike() && !p->isFragile()) { // 排除陷阱和易碎板
                if (p->y() <= highestSafeY) {
                    highestSafeY = p->y();
                    highestSafeX = p->x();
                }
            }
        }

        // 3. 计算这个安全基准点在第几列 (0, 1, 2, 3)
        int lastCol = 0;
        qreal minDiff = 9999;
        for (int i = 0; i < 4; ++i) {
            if (qAbs(highestSafeX - cols[i]) < minDiff) {
                minDiff = qAbs(highestSafeX - cols[i]);
                lastCol = i;
            }
        }

        // 4. 下一层的保底平台只能在它的【左、中、右】生成 (跨度不超过1列)
        int safeCol = lastCol + QRandomGenerator::global()->bounded(-1, 2);
        if (safeCol < 0) safeCol = 0;
        if (safeCol > 3) safeCol = 3;

        QList<qreal> availableX = cols;
        qreal safeX = cols[safeCol];
        availableX.removeOne(safeX); // 把保底位置拿走

        // ===================================
        // 核心：优先生成保底平台，打通生命通道
        // ===================================
        Platform *safeP;
        if (QRandomGenerator::global()->generateDouble() < 0.2) {
            safeP = new SuperPlatform(); // 保底偶尔给个弹簧板爽一下
        } else {
            safeP = new NormalPlatform();
        }
        safeP->setPos(safeX, minY - 150);
        m_platforms.append(safeP);
        m_scene->addItem(safeP);

        // 💡 隐形引导：在安全平台上方高概率刷金币，引导玩家走正确的路
        if (QRandomGenerator::global()->generateDouble() < 0.6) {
            Coin *c = new Coin();
            c->setPos(safeX + 50, minY - 180);
            m_coins.append(c);
            m_scene->addItem(c);
        }

        // ===================================
        // 动态难度系统：根据灵感值调节地狱级别！
        // ===================================
        int maxExtra = 3;       // 默认最多生成几个额外板子
        double spikeProb = 0.30; // 默认尖刺概率 30%
        double fragProb = 0.60;  // 默认尖刺+易碎总概率 60%

        if (m_score >= 300) {
            // 💀 地狱模式 (300分以上)：几乎没有旁路，一旦跳错必死！
            maxExtra = 0;       // 额外板子数量降为 0！周围全空！
            spikeProb = 0.80;   // 如果非要刷，80%是刺！
        }
        else if (m_score >= 150) {
            // 😈 困难模式 (150分以上)：板子变少，满地是坑
            maxExtra = 1;       // 最多 1 个额外板子
            spikeProb = 0.60;
            fragProb = 0.85;    // 极高概率踩碎
        }
        else if (m_score >= 50) {
            // ⚠️ 警告模式 (50分以上)：逐渐收网
            maxExtra = 2;
            spikeProb = 0.45;
        }

        // 随机生成 0 到 maxExtra 个干扰平台
        int extraCount = QRandomGenerator::global()->bounded(0, maxExtra + 1);

        for (int i = 0; i < extraCount; ++i) {
            // 如果备选位置空了，就直接跳出循环防崩溃
            if (availableX.isEmpty()) break;

            int randIdx = QRandomGenerator::global()->bounded(availableX.size());
            qreal px = availableX.takeAt(randIdx);

            Platform *p;
            double randVal = QRandomGenerator::global()->generateDouble();

            // 使用我们刚才动态计算出来的阴间概率
            if (randVal < spikeProb) {
                p = new SpikePlatform();
            } else if (randVal < fragProb) {
                p = new FragilePlatform();
            } else {
                p = new NormalPlatform();
            }

            p->setPos(px, minY - 150);
            m_platforms.append(p);
            m_scene->addItem(p);

            // 随机生成零散的道具 (高难度下金币也会变少，逼迫玩家去危险边缘试探)
            double itemRand = QRandomGenerator::global()->generateDouble();
            if (itemRand < 0.05) {
                Magnet *m = new Magnet();
                m->setPos(px + 50, minY - 180);
                m_magnets.append(m);
                m_scene->addItem(m);
            } else if (itemRand < 0.2 && m_score < 150) {
                // 150分以后，干扰板上不再刷金币，想白嫖不可能了！
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
        m_magnetText->setPlainText(QString("灵感爆发: %1s").arg(m_magnetTimeLeft));
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
        // ==========================================
        // --- 新增：切换游戏结束专属背景 ---
        // ==========================================
        // 加载你的 Game Over 背景图（比如破防的灰暗自习室、电脑蓝屏等）
        QPixmap gameoverBg(":/new/prefix2/background3.png");
        if (!gameoverBg.isNull()) {
            m_scene->setBackgroundBrush(QBrush(gameoverBg.scaled(SCREEN_W, SCREEN_H, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
        } else {
            // 如果图片加载失败或没准备好，就用深红色/纯黑色渐变作为保底效果
            QLinearGradient gradient(0, 0, 0, SCREEN_H);
            gradient.setColorAt(0.0, QColor(50, 0, 0)); // 暗红色
            gradient.setColorAt(1.0, QColor(0, 0, 0));  // 纯黑色
            m_scene->setBackgroundBrush(QBrush(gradient));
        }
        QApplication::processEvents();
        m_gameTimer->stop();
        m_gameTime.invalidate();
        m_gameisover=true;
        if (m_bgmSound) {
            m_bgmSound->stop(); // 游戏结束，停止播放背景音乐
        }
        if (m_loseSound) m_loseSound->play(); // 播放死亡音效

        // 根据位置判断是怎么死的，给出不同剧情
        QString deathStory;
        if (m_ball->y() > DEAD_LINE + SCREEN_H + 50) {
            deathStory = "掉出可用内存边界。\n\nStack Overflow ";
        } else {
            deathStory = "撞上致命 Bug。\n\nSegmentation Fault";
        }

        showStoryPopup("💥运行崩溃", deathStory, 2);
        checkDeathAchievements("SPIKE");
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
    m_scoreText->setPlainText(QString("灵感值: %1").arg(m_score));
    m_hpText->setPlainText(QString("精力: %1").arg(m_ball->getHp()));
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
            QStringList parts = line.split(" - 灵感值: ");
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
    QString newLine = currentTime + " - 灵感值: " + QString::number(m_score);
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
void gameview::unlockAchievement(QString key, QString name)
{
    QSettings settings("MyGame", "DDL_Survival");

    if (!settings.value(key, false).toBool()) {
        settings.setValue(key, true);
        if (m_achSound) m_achSound->play();

        qint64 pauseStart = m_gameTime.elapsed();

        // 极简成就文案
        QString story = QString("新成就已记录：\n【%1】").arg(name);
        showStoryPopup("✨ ACHIEVEMENT UNLOCKED", story, 3);

        m_totalTime += (m_gameTime.elapsed() - pauseStart) / 1000;
    }
}

void gameview::checkDeathAchievements(QString deathType)
{
    // 1. 判断高分成就
    if (m_score >= 500) unlockAchievement("ACH_FULLSTACK", "满绩卷王 (4.0 GPA)");
    else if (m_score >= 200) unlockAchievement("ACH_AC", "一遍过 (Accepted)");

    // 2. 判断零分
    if (m_score == 0) unlockAchievement("ACH_RMRF", "灵感枯竭 (Null Pointer)");

    // 3. 判断死法
    if (deathType == "TIMEOUT") {
        unlockAchievement("ACH_TLE", "超时 (Time Limit Exceeded)");
    }
    else if (deathType == "FALL") {
        unlockAchievement("ACH_STACKOVERFLOW", "栈溢出 (Stack Overflow)");
    }
    else if (deathType == "SPIKE") {
        if (m_gameTime.elapsed() < 5000) { // 开局5秒内被刺死
            unlockAchievement("ACH_SEGFAULT", "段错误 (Segmentation Fault)");
        }
    }
}
// themeType: 1=胜利(绿色), 2=失败/死亡(红色), 3=成就(蓝色)
void gameview::showStoryPopup(QString title, QString story, int themeType)
{
    QDialog dialog(this);
    dialog.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    // 关键1：先把底层 Dialog 设为全透明，防止圆角外面出现丑陋的黑边
    dialog.setAttribute(Qt::WA_TranslucentBackground);
    dialog.setFixedSize(400, 220);

    QString borderColor, btnHover;
    if (themeType == 1) { // 胜利
        borderColor = "#10b981"; btnHover = "#059669";
    } else if (themeType == 2) { // 失败/死亡
        borderColor = "#ef4444"; btnHover = "#dc2626";
    } else { // 成就
        borderColor = "#3b82f6"; btnHover = "#2563eb";
    }

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->setContentsMargins(0, 0, 0, 0); // 取消边缘留白

    // ==========================================
    // 关键2：焊死一块实心钢板 (QFrame) 作为真正的底色背景
    // ==========================================
    QFrame *bgFrame = new QFrame(&dialog);
    bgFrame->setStyleSheet(QString(
                               "QFrame { background-color: #1a0b2e; border: 4px solid %1; border-radius: 12px; }" // 绝对不透光的深紫！
                               "QLabel#Title { color: %1; font-size: 24px; font-weight: bold; font-family: 'Microsoft YaHei'; border: none; background: transparent; }"
                               "QLabel#Story { color: #ffffff; font-size: 18px; font-family: 'Microsoft YaHei'; border: none; background: transparent; }"
                               "QPushButton { background: #2d1e40; color: %1; border: 2px solid %1; border-radius: 6px; font-size: 16px; font-weight: bold; padding: 6px; }"
                               "QPushButton:hover { background: %1; color: #1a0b2e; }"
                               ).arg(borderColor));
    mainLayout->addWidget(bgFrame);

    // 把所有文字和按钮都放在这块实心钢板 (bgFrame) 上
    QVBoxLayout *frameLayout = new QVBoxLayout(bgFrame);
    frameLayout->setContentsMargins(30, 25, 30, 20);
    frameLayout->setSpacing(10);

    QLabel *lblTitle = new QLabel(title, bgFrame);
    lblTitle->setObjectName("Title");
    lblTitle->setAlignment(Qt::AlignCenter);
    frameLayout->addWidget(lblTitle);

    QLabel *lblStory = new QLabel(story, bgFrame);
    lblStory->setObjectName("Story");
    lblStory->setWordWrap(true);
    lblStory->setAlignment(Qt::AlignCenter);
    frameLayout->addWidget(lblStory);

    frameLayout->addStretch();
    QPushButton *btn = new QPushButton("确认 (Enter)", bgFrame);
    frameLayout->addWidget(btn);
    connect(btn, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.exec();
}

// ==========================================
// 游戏暂停核心逻辑
// ==========================================
void gameview::pauseGame()
{
    // 如果游戏已经结束，就别暂停了
    if (m_gameisover) return;

    // 1. 物理层面的冻结（停掉所有计时器）
    m_gameTimer->stop();
    if (m_magnetTimer->isActive()) m_magnetTimer->stop();

    // 💡 黑科技：把已经流失的时间扣掉，只保存剩下的秒数
    m_totalTime -= (m_gameTime.elapsed() / 1000);

    // 2. 绘制深紫色实心暂停面板
    QDialog dialog(this);
    dialog.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    dialog.setAttribute(Qt::WA_TranslucentBackground);
    dialog.setFixedSize(300, 360);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QFrame *bgFrame = new QFrame(&dialog);
    bgFrame->setStyleSheet(
        "QFrame { background-color: #1a0b2e; border: 4px solid #b285e6; border-radius: 12px; }"
        "QLabel { color: #d8b4ff; font-size: 26px; font-weight: bold; font-family: 'Microsoft YaHei'; border: none; background: transparent; }"
        "QPushButton { background: #2d1e40; color: #ffffff; border: 2px solid #b285e6; border-radius: 6px; font-size: 18px; font-weight: bold; padding: 12px; }"
        "QPushButton:hover { background: #b285e6; color: #1a0b2e; }"
        );
    mainLayout->addWidget(bgFrame);

    QVBoxLayout *frameLayout = new QVBoxLayout(bgFrame);
    frameLayout->setContentsMargins(30, 25, 30, 25);
    frameLayout->setSpacing(15);

    QLabel *title = new QLabel("⏸️ 游 戏 暂 停", bgFrame);
    title->setAlignment(Qt::AlignCenter);
    frameLayout->addWidget(title);

    frameLayout->addStretch();

    QPushButton *btnContinue = new QPushButton("继续 (Continue)", bgFrame);
    QPushButton *btnRestart = new QPushButton("重开 (Restart)", bgFrame);
    QPushButton *btnMenu = new QPushButton("返回主页 (Menu)", bgFrame);

    frameLayout->addWidget(btnContinue);
    frameLayout->addWidget(btnRestart);
    frameLayout->addWidget(btnMenu);

    // 利用 done() 传出不同的退出码
    connect(btnContinue, &QPushButton::clicked, [&](){ dialog.done(0); });
    connect(btnRestart, &QPushButton::clicked, [&](){ dialog.done(1); });
    connect(btnMenu, &QPushButton::clicked, [&](){ dialog.done(2); });

    int ret = dialog.exec(); // 阻塞游戏进程

    // 3. 根据玩家的选择进行恢复
    if (ret == 0) {
        // 【继续游戏】：重新启动计时器
        m_gameTime.restart(); // 重新开始计算间隔
        m_gameTimer->start(16);
        if (m_isMagnetActive) m_magnetTimer->start(1000);

    } else if (ret == 1) {
        // 【重新开局】：调用重置函数
        restartGame();

    } else if (ret == 2) {
        // 【返回主页】：彻底关掉当前游戏
        emit backToMenu();
        this->close();
    }
}

// ==========================================
// 重新开局核心逻辑
// ==========================================
void gameview::restartGame()
{
    // 1. 清理上一局留在画面上的所有“遗物”
    qDeleteAll(m_platforms); m_platforms.clear();
    qDeleteAll(m_coins);     m_coins.clear();
    qDeleteAll(m_magnets);   m_magnets.clear();
    m_lastSavePlatform = nullptr;

    // 2. 恢复最初的数值
    m_score = 0;
    m_ball->setHp(m_initialHp);    // 恢复玩家开局设好的血量
    m_totalTime = m_initialTime;   // 恢复玩家开局设好的时间
    m_isMagnetActive = false;
    m_magnetTimeLeft = 0;
    m_magnetText->setPlainText("");

    // 3. 恢复小球的状态
    m_ball->setPos(SCREEN_W/2 - 15, SCREEN_H - 100);
    m_ball->resetVelocity();
    m_leftPress = false;
    m_rightPress = false;
    m_gameisover = false;

    // 4. 重新铺好开局的地砖
    _init_platform_coin();

    // 5. 重新发车！
    m_gameTime.restart();
    m_gameTimer->start(16);

    // 如果因为上局死掉导致 BGM 停了，给它重新放起来
    if (m_bgmSound && !m_bgmSound->isPlaying()) {
        m_bgmSound->play();
    }
}

// ==========================================
// 掉血飘字特效
// ==========================================
void gameview::showDamageEffect(qreal x, qreal y)
{
    // 1. 创建红色掉血文字
    QGraphicsTextItem *hurtText = new QGraphicsTextItem("-1 精力");
    hurtText->setDefaultTextColor(QColor(255, 50, 50)); // 醒目的亮红色
    hurtText->setFont(QFont("Microsoft YaHei", 20, QFont::Bold));

    // 把它放在小球的正上方一点
    hurtText->setPos(x, y - 40);
    hurtText->setZValue(999); // 保证数字浮在最上层，不被板子挡住
    m_scene->addItem(hurtText);

    // 2. 创建一个临时定时器来做动画
    QTimer *fadeTimer = new QTimer(this);

    // 每次触发定时器：文字上移，并降低透明度
    connect(fadeTimer, &QTimer::timeout, [=]() mutable {
        hurtText->moveBy(0, -2); // 每次向上飘 2 个像素

        qreal currentOpacity = hurtText->opacity() - 0.05; // 每次变透明 5%

        if (currentOpacity <= 0) {
            // 完全透明了，停止动画，销毁文字和定时器本身
            fadeTimer->stop();
            m_scene->removeItem(hurtText);
            delete hurtText;
            fadeTimer->deleteLater();
        } else {
            hurtText->setOpacity(currentOpacity);
        }
    });

    // 每 30 毫秒刷新一帧，实现 30FPS 的丝滑飘字
    fadeTimer->start(30);
}