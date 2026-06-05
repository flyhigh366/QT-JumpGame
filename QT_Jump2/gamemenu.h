#ifndef GAMEMENU_H
#define GAMEMENU_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class QPushButton;

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog() override;
    void Showme();

private slots:
    void on_button_scoresheet_clicked();
    void selectSkinDog();
    void selectSkinCat();
    void selectSkinPanda();

private:
    Ui::Dialog *ui;
    QPushButton *m_btnDog;
    QPushButton *m_btnCat;
    QPushButton *m_btnPanda;
};
#endif // GAMEMENU_H