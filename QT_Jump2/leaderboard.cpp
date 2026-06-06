#include "leaderboard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>

Leaderboard::Leaderboard(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("🏆 DDL 战神名人堂");
    // 1. 把窗口变大，比例更修长优雅
    resize(600, 750);

    // ==========================================
    // 2. 注入顶级 QSS：渐变背景与玻璃态UI
    // ==========================================
    this->setStyleSheet(R"(
        /* 对话框使用深邃的紫蓝到纯黑的渐变色 */
        QDialog {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                        stop:0 #1a0b2e,
                                        stop:0.5 #0d142b,
                                        stop:1 #000000);
            border: 2px solid #5a3d8a;
            border-radius: 15px;
        }

        /* 文本框使用半透明暗色，制造高级的“磨砂玻璃”悬浮感 */
        QTextBrowser {
            background-color: rgba(20, 25, 40, 180);
            border: 2px solid rgba(100, 150, 255, 50);
            border-radius: 12px;
            padding: 25px;
            font-family: "Microsoft YaHei";
            font-size: 18px; /* 字体调大 */
        }

        /* 底部按钮的酷炫流光效果 */
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                        stop:0 #ff416c,
                                        stop:1 #ff4b2b);
            color: white;
            border: none;
            border-radius: 12px;
            font-family: "Microsoft YaHei";
            font-size: 18px;
            font-weight: bold;
            padding: 12px 25px;
            min-height: 45px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                        stop:0 #ff4b2b,
                                        stop:1 #ff416c);
            border: 1px solid #ffaaaa;
        }
        QPushButton:pressed {
            background: #cc3333;
            padding-top: 15px;
            padding-bottom: 9px;
        }
    )");

    // 3. 创建控件
    m_textBrowser = new QTextBrowser(this);
    // 隐藏滚动条背景，让它看起来更干净
    m_textBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_btnBack = new QPushButton("返回主菜单", this);
    m_btnStart = new QPushButton("🔥 再次挑战 DDL", this);

    // 4. 布局排版（四周留白加大，增加呼吸感）
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(35, 40, 35, 35);
    mainLayout->addWidget(m_textBrowser);

    // 在文本和按钮之间加一点间距
    mainLayout->addSpacing(20);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(m_btnBack);
    btnLayout->addSpacing(30); // 两个按钮之间拉开距离
    btnLayout->addWidget(m_btnStart);
    mainLayout->addLayout(btnLayout);

    // 5. 信号槽连接
    connect(m_btnBack, &QPushButton::clicked, this, &Leaderboard::reject);
    connect(m_btnStart, &QPushButton::clicked, this, &Leaderboard::accept);

    // 6. 读取历史成绩
    loadScores();
}

void Leaderboard::loadScores()
{
    QFile file("scores.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);

        // 使用 HTML 来构建富文本，这样可以对不同名次单独设置颜色和字号
        QString htmlString = "<div style='text-align: center; margin-bottom: 20px;'>"
                             "<span style='font-size: 26px; font-weight: bold; color: #E2E8F0; letter-spacing: 2px;'>"
                             "✦ 荣 誉 殿 堂 ✦</span><br><br></div>";
        int rank = 1;

        while (!in.atEnd() && rank <= 10) {
            QString line = in.readLine();
            QString prefix;
            QString color;

            // 给前三名分配闪耀的独立颜色和字体大小
            if (rank == 1) {
                prefix = "🥇 [卷王之王]";
                color = "#FFD700"; // 金色
            } else if (rank == 2) {
                prefix = "🥈 [学术巨佬]";
                color = "#C0C0C0"; // 银色
            } else if (rank == 3) {
                prefix = "🥉 [踩线战神]";
                color = "#CD7F32"; // 铜色
            } else {
                prefix = QString(" 🎗️ 第 %1 名:").arg(rank);
                color = "#8BE9FD"; // 赛博青色
            }

            // 用 HTML 标签拼接每一行，让称号变色，分数本体保持白色
            htmlString += QString("<div style='margin-bottom: 12px; font-size: 18px;'>"
                                  "<span style='color: %1; font-weight: bold;'>%2</span>"
                                  "&nbsp;&nbsp;&nbsp;&nbsp;"
                                  "<span style='color: #F8F8F2;'>%3</span>"
                                  "</div>").arg(color, prefix, line);
            rank++;
        }

        if (rank == 1) { // 没读到任何数据
            m_textBrowser->setHtml("<div style='text-align: center; color: #6272A4; font-size: 20px; margin-top: 100px;'>"
                                   "暂无记录！<br><br>快去创造你的第一次 DDL 奇迹吧！</div>");
        } else {
            // 使用 setHtml 而不是 setText，激活富文本渲染
            m_textBrowser->setHtml(htmlString);
        }
        file.close();
    } else {
        m_textBrowser->setHtml("<div style='text-align: center; color: #6272A4; font-size: 20px; margin-top: 100px;'>"
                               "暂无记录！<br><br>快去创造你的第一次 DDL 奇迹吧！</div>");
    }
}