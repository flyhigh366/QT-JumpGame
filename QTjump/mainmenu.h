 #ifndef MAINMENU_H
#define MAINMENU_H
#include <QWidget>
#include <QPushButton>

class MainMenu : public QWidget
{
    Q_OBJECT
public:
    explicit MainMenu(QWidget *parent = nullptr);
signals:
    void startGame();
    void quitApp();
};
#endif // MAINMENU_H