#ifndef ACHIEVEMENTS_H
#define ACHIEVEMENTS_H

#include <QDialog>

namespace Ui {
class achievements;
}

class achievements : public QDialog
{
    Q_OBJECT

public:
    explicit achievements(QWidget *parent = nullptr);
    ~achievements();


private slots:
    void on_btn_back_clicked();

    void on_btn_start_clicked();

private:
    Ui::achievements *ui;
};

#endif // ACHIEVEMENTS_H
