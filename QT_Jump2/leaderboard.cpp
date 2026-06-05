#include "leaderboard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>

Leaderboard::Leaderboard(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("历史战绩 / 排行榜");
    resize(300, 400); // 窗口大小

    // 1. 创建控件
    m_textBrowser = new QTextBrowser(this);
    m_textBrowser->setFont(QFont("Arial", 12));

    m_btnBack = new QPushButton("返回主菜单", this);
    m_btnStart = new QPushButton("直接开始游戏", this);

    // 2. 布局排版
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_textBrowser);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(m_btnBack);
    btnLayout->addWidget(m_btnStart);
    mainLayout->addLayout(btnLayout);

    // 3. 信号槽连接（最关键的一步）
    // 点击“返回主菜单”，关闭窗口并返回 Rejected
    connect(m_btnBack, &QPushButton::clicked, this, &Leaderboard::reject);
    // 点击“直接开始游戏”，关闭窗口并返回 Accepted
    connect(m_btnStart, &QPushButton::clicked, this, &Leaderboard::accept);

    // 4. 读取历史成绩
    loadScores();
}


void Leaderboard::loadScores()
{
    QFile file("scores.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString displayString;
        int rank = 1;

        // 逐行读取，并加上名次标识
        while (!in.atEnd()) {
            QString line = in.readLine();
            displayString += QString("第 %1 名:  %2\n").arg(rank).arg(line);
            rank++;
        }

        if (displayString.isEmpty()) {
            m_textBrowser->setText("暂无记录！快去创造历史吧！");
        } else {
            m_textBrowser->setText(displayString);
        }
        file.close();
    } else {
        m_textBrowser->setText("暂无记录！快去创造历史吧！");
    }
}
