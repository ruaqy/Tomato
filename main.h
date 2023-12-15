#ifndef MAIN_H
#define MAIN_H

#include <QMenu>
#include <QTimer>
#include <QWidget>
#include <QAction>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QCloseEvent>
#include <QSystemTrayIcon>

#include "setting.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Main;
}
QT_END_NAMESPACE

struct StateMachine{
    int count;
    int now_s;
    int max_s;
    WorkStatus now_status;
    WorkStatus next_status;
};

class Main : public QWidget
{
    Q_OBJECT
    
    ApaSetting appearance = ApaSetting();
    RotSetting routine;
    GenSetting general;

    QTimer timer = QTimer(this);


    StateMachine state;

    // System Icon
    QSystemTrayIcon sysTrayIcon;
    QMenu menuTray;
    QAction actQuit;

public:
    Main(QWidget *parent = nullptr);
    ~Main();
protected:
    void mousePressEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void closeEvent(QCloseEvent* event);

private:
    Ui::Main *ui;
    
    void setTopWindow();
    void initVariance();
    void initSysTrayIcon();

    void updateNumber();
    void updateTomato();
    void updateGeneral();
private slots:
    void quit();
    void increase();
    void onSysTrayIconActived(QSystemTrayIcon::ActivationReason reason);
};
#endif // MAIN_H
