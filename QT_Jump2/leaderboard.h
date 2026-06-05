#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <QDialog>
#include <QTextBrowser>
#include <QPushButton>

class Leaderboard : public QDialog
{
    Q_OBJECT
public:
    explicit Leaderboard(QWidget *parent = nullptr);

private:
    QTextBrowser *m_textBrowser; // 用来显示分数的文本框
    QPushButton *m_btnBack;      // 返回菜单按钮
    QPushButton *m_btnStart;     // 直接开始按钮

private slots:
    void loadScores();           // 读取分数的函数
};

#endif // LEADERBOARD_H
