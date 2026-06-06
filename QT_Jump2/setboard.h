#ifndef SETBOARD_H
#define SETBOARD_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class setboard;
}

class setboard : public QDialog
{
    Q_OBJECT

public:
    explicit setboard(QWidget *parent = nullptr);
    ~setboard();
    // 开放给外部（如 gamemenu）获取设置参数的接口

    int getHp() const;
    int getTime() const;

private slots:
    void on_btnStart_clicked();   // 开始游戏按钮
    void on_btnReturn_clicked();  //


private:
    Ui::setboard *ui;
    int m_hp;
    int m_time;
};

#endif // SETBOARD_H
