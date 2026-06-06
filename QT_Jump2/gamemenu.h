#ifndef GAMEMENU_H
#define GAMEMENU_H

#include <QDialog>
#include <QSettings> // ⚠️ 第一步：必须加上这个头文件！

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog() override;
    void Showme();

    // ==========================================
    // ⚠️ 第二步：修改获取数据的接口，直接去档案里拿最新数据！
    // ==========================================
    int getHp() const {
        QSettings settings("MyGame", "DDL_Survival");
        return settings.value("PlayerHp", 3).toInt();
    }

    int getTime() const {
        QSettings settings("MyGame", "DDL_Survival");
        return settings.value("GameTime", 150).toInt();
    }

    int getSkin() const { return m_skin; }

private slots:
    void on_button_scoresheet_clicked();
    void on_pushButton_clicked();
    void on_btn_skinDefault_clicked();
    void on_btn_skinDog_clicked();
    void on_btn_skinCat_clicked();
    void on_btn_skinPanda_clicked();
    void on_btn_achievements_clicked();

private:
    Ui::Dialog *ui;
    int m_skin = 0;
    void saveSkin();

    // ⚠️ 第三步：请务必检查这里！
    // 以前这里写的 int m_hp = 3; 和 int m_time = 150; 必须删掉！不要留着它们了！
};

#endif // GAMEMENU_H