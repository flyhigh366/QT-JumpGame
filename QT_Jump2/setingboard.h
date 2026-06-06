#ifndef SETINGBOARD_H
#define SETINGBOARD_H

#include <QDialog>

namespace Ui {
class Setingboard;
}

class Setingboard: public QDialog
{
    Q_OBJECT

public:
    explicit Setingboard(QWidget *parent = nullptr);
    ~Setingboard();

private:
    Ui::D*ui;
};

#endif // SETINGBOARD_H
