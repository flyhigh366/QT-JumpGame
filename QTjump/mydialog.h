#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class myDialog;
}
QT_END_NAMESPACE

class myDialog : public QDialog
{
    Q_OBJECT

public:
    explicit myDialog(QWidget *parent = nullptr);
    ~myDialog() override;

private:
    Ui::myDialog *ui;
};
#endif // MYDIALOG_H
