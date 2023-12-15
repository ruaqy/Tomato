#include "main.h"
#include "./ui_main.h"
#include "setting.h"

#include <QMenu>
#include <QPainter>
#include <QSettings>
#include <QDateTime>
#include <QSystemTrayIcon>

#define SAMPLE_TIME 100
#define MULTIP_TIME 10

Main::Main(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Main)
{
    ui->setupUi(this);

    initVariance();
    initSysTrayIcon();

    this->setWindowTitle(tr("Tomato"));

    connect(&timer, SIGNAL(timeout()), this, SLOT(increase()));

    if (general.autoStartTimer)
        timer.start(SAMPLE_TIME);

    updateGeneral();
    updateTomato();
}

Main::~Main()
{
    delete ui;
}

void Main::closeEvent(QCloseEvent* event)
{
    this->hide();	 // hidden windows
    if(general.showStartPanel)
        event->ignore(); // ignore the close event.
}

void Main::wheelEvent(QWheelEvent *event){
    if(event->angleDelta().y()>0)
        state.now_s += 20*MULTIP_TIME;
    else
        state.now_s -= 10*MULTIP_TIME;
}

void Main::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        if (timer.isActive())
        {
            timer.stop();
            ui->label->setText("STOP");
        }
        else
            timer.start(SAMPLE_TIME);

    }
    else if (event->button() == Qt::RightButton)
    {
        bool run = false;

        if (timer.isActive())
        {
            timer.stop();
            run = true;
        }

        Setting setting(routine, general, appearance, this);
        setting.exec();

        updateTomato();
        updateGeneral();

        if(run)
            timer.start(SAMPLE_TIME);


    }
}

void Main::initVariance(){
    QSettings settings("myapp.ini", QSettings::IniFormat);

    settings.beginGroup("General");
    general = {settings.value("autoStartTimer", false).toBool(),
               settings.value("miniAfterStart", false).toBool(),
               settings.value("alwaysOnTheTop", false).toBool(),
               settings.value("restartRoutine", false).toBool(),
               settings.value("popUpEachEpoch", false).toBool(),
               settings.value("showStartPanel", false).toBool()};
    settings.endGroup();

    settings.beginGroup("Time");
    routine = {
        settings.value("focus", 25*60).toInt(),
        settings.value("short_break", 5*60).toInt(),
        settings.value("long_break", 15*60).toInt(),
        settings.value("round", 4).toInt()
    };
    settings.endGroup();

    state = {1, 0, routine.work*MULTIP_TIME, Work, SBreak};

    settings.beginGroup("Appearance");
    appearance.wk_bg = QColor(settings.value("wk_bg", appearance.wk_bg.name()).toString().mid(1).toUInt(NULL, 16));
    appearance.wk_ig = QColor(settings.value("wk_ig", appearance.wk_ig.name()).toString().mid(1).toUInt(NULL, 16));
    appearance.wk_fg = QColor(settings.value("wk_fg", appearance.wk_fg.name()).toString().mid(1).toUInt(NULL, 16));
    appearance.sb_bg = QColor(settings.value("sb_bg", appearance.sb_bg.name()).toString().mid(1).toUInt(NULL, 16));
    appearance.sb_ig = QColor(settings.value("sb_ig", appearance.sb_ig.name()).toString().mid(1).toUInt(NULL, 16));
    appearance.sb_fg = QColor(settings.value("sb_fg", appearance.sb_fg.name()).toString().mid(1).toUInt(NULL, 16));
    appearance.lb_bg = QColor(settings.value("lb_bg", appearance.lb_bg.name()).toString().mid(1).toUInt(NULL, 16));
    appearance.lb_ig = QColor(settings.value("lb_ig", appearance.lb_ig.name()).toString().mid(1).toUInt(NULL, 16));
    appearance.lb_fg = QColor(settings.value("lb_fg", appearance.lb_fg.name()).toString().mid(1).toUInt(NULL, 16));
    appearance.font.setFamily(settings.value("font_family", "Arial").toString());
    appearance.font.setPointSize(settings.value("font_size", 64).toInt());
    settings.endGroup();
}

void Main::initSysTrayIcon()
{
    sysTrayIcon.setIcon(QIcon("Resources/ico.ico"));
    sysTrayIcon.show();

    actQuit.setText(tr("Quit"));
    actQuit.setParent(&menuTray);

    menuTray.addAction(&actQuit);

    sysTrayIcon.setToolTip(tr("Tomato"));
    sysTrayIcon.setContextMenu(&menuTray);

    connect(&sysTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(onSysTrayIconActived(QSystemTrayIcon::ActivationReason)));

    connect(&actQuit, SIGNAL(triggered()), this, SLOT(quit()));
}

void Main::quit(){
    qApp->quit();
}

void Main::onSysTrayIconActived(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::Trigger://单击
    case QSystemTrayIcon::DoubleClick://双击
    {
        if (isHidden())
        {
            show();
        }
        else
        {
            setTopWindow();
        }
        break;
    }
    default:
        break;
    }
}

void Main::setTopWindow()
{
    if (general.alwaysOnTheTop){

        Qt::WindowStates winStatus = Qt::WindowNoState;
        if (windowState() & Qt::WindowMaximized)
        {
            winStatus = Qt::WindowMaximized;
        }
        setWindowState(Qt::WindowActive | winStatus);
        activateWindow();
        raise();
    }
}

void Main::increase(){
    state.now_s++;

    if(state.now_s>=state.max_s){

        if(general.restartRoutine && state.now_status==LBreak)
            timer.stop();

        if(isHidden())
            show();

        state.now_status = state.next_status;

        switch(state.next_status){
        case Work:
            if (state.count!=0 && state.count % (routine.round-1)==0)
                state.next_status=LBreak;
            else
                state.next_status=SBreak;
            state.max_s = routine.work*MULTIP_TIME;

            if (general.miniAfterStart && general.showStartPanel)
                hide();

            sysTrayIcon.showMessage(tr("Work"), tr("It's time to focus."), QSystemTrayIcon::Information, 5000);
            state.count++;
            break;
        case SBreak:
            state.next_status=Work;
            state.max_s = routine.short_break*MULTIP_TIME;
            sysTrayIcon.showMessage(tr("Relax"), tr("Let's relaxing..."), QSystemTrayIcon::Information, 5000);
            break;
        case LBreak:
            state.next_status=Work;
            state.max_s = routine.long_break*MULTIP_TIME;
            sysTrayIcon.showMessage(tr("Relax"), tr("To have a coffee or some meating."), QSystemTrayIcon::Information, 5000);
            break;
        };

        state.now_s = 0;
    }

    ui->widget->setPersent(state.now_s, state.max_s);
    updateTomato();
    updateNumber();

    ui->widget->update();
}

void Main::updateTomato(){
    ui->label->setFont(appearance.font);

    switch (state.now_status) {
    case Work:
        ui->widget->setColor(appearance.wk_fg, appearance.wk_ig, appearance.wk_bg);
        ui->label->setStyleSheet(QString("color: ")+appearance.wk_fg.name()+";");
        break;
    case SBreak:
        ui->widget->setColor(appearance.sb_fg, appearance.sb_ig, appearance.sb_bg);
        ui->label->setStyleSheet(QString("color: ")+appearance.sb_fg.name()+";");
        break;
    case LBreak:
        ui->widget->setColor(appearance.lb_fg, appearance.lb_ig, appearance.lb_bg);
        ui->label->setStyleSheet(QString("color: ")+appearance.lb_fg.name()+";");
        break;
    default:
        break;
    }
}

void Main::updateGeneral(){
    if(general.alwaysOnTheTop)
    {
        this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
        this->showNormal();
    }else{
        this->setWindowFlags(this->windowFlags() & ~Qt::WindowStaysOnTopHint);
        this->showNormal();
    }

    if(general.showStartPanel)
        sysTrayIcon.show();
    else
        sysTrayIcon.hide();
}

void Main::updateNumber(){
    QDateTime datetime;

    int lease = (state.max_s - state.now_s)/MULTIP_TIME;

    this->ui->label->setText(datetime.fromSecsSinceEpoch(lease).toString("mm:ss"));
}



