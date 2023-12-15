#include "setting.h"
#include "ui_setting.h"

#include <QPushButton>
#include <QColorDialog>
#include <QSettings>

Setting::Setting(RotSetting &current_routine, GenSetting &current_general, ApaSetting& current, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Setting)
    , older(current)
    , newer(current)
    , routine(current_routine)
    , general(current_general)
{
    ui->setupUi(this);

    this->updateRoutine();
    this->updateGeneral();
    this->updateAppearance();

    connect(ui->wk_bg, SIGNAL(clicked()), this, SLOT(selectColor()));
    connect(ui->wk_ig, SIGNAL(clicked()), this, SLOT(selectColor()));
    connect(ui->wk_fg, SIGNAL(clicked()), this, SLOT(selectColor()));
    connect(ui->sb_bg, SIGNAL(clicked()), this, SLOT(selectColor()));
    connect(ui->sb_ig, SIGNAL(clicked()), this, SLOT(selectColor()));
    connect(ui->sb_fg, SIGNAL(clicked()), this, SLOT(selectColor()));
    connect(ui->lb_bg, SIGNAL(clicked()), this, SLOT(selectColor()));
    connect(ui->lb_ig, SIGNAL(clicked()), this, SLOT(selectColor()));
    connect(ui->lb_fg, SIGNAL(clicked()), this, SLOT(selectColor()));

    connect(ui->font_fm, SIGNAL(currentFontChanged(QFont)), this, SLOT(selectFont(QFont)));

    connect(ui->apply, SIGNAL(accepted()), this, SLOT(save()));
    connect(ui->apply, SIGNAL(rejected()), this, SLOT(close()));

}

void Setting::updateRoutine(){
    this->ui->focus->setValue(routine.work/60);
    this->ui->round->setValue(routine.round);
    this->ui->short_break->setValue(routine.short_break/60);
    this->ui->long_break->setValue(routine.long_break/60);
}

void Setting::updateGeneral(){
    this->ui->alwaysOnTheTop->setChecked(general.alwaysOnTheTop);
    this->ui->autoStartTimer->setChecked(general.autoStartTimer);
    this->ui->miniAfterStart->setChecked(general.miniAfterStart);
    this->ui->alwaysOnTheTop->setChecked(general.alwaysOnTheTop);
    this->ui->restartRoutine->setChecked(general.restartRoutine);
    this->ui->popUpEachEpoch->setChecked(general.popUpEachEpoch);
    this->ui->showStartPanel->setChecked(general.showStartPanel);
}

void Setting::updateAppearance(){
    QString style = "background-color: ";
    this->ui->wk_bg->setStyleSheet(style + newer.wk_bg.name());
    this->ui->wk_ig->setStyleSheet(style + newer.wk_ig.name());
    this->ui->wk_fg->setStyleSheet(style + newer.wk_fg.name());
    this->ui->sb_bg->setStyleSheet(style + newer.sb_bg.name());
    this->ui->sb_ig->setStyleSheet(style + newer.sb_ig.name());
    this->ui->sb_fg->setStyleSheet(style + newer.sb_fg.name());
    this->ui->lb_bg->setStyleSheet(style + newer.lb_bg.name());
    this->ui->lb_ig->setStyleSheet(style + newer.lb_ig.name());
    this->ui->lb_fg->setStyleSheet(style + newer.lb_fg.name());

    this->ui->font_fm->setCurrentFont(newer.font);
    this->ui->font_sz->setValue(newer.font.pointSize());
}

void Setting::selectColor(){
    QPushButton* btn = (QPushButton*)sender();

    QColor color = QColorDialog::getColor();

    if (btn==ui->wk_bg) newer.wk_bg=color;
    else if(btn==ui->wk_ig) newer.wk_ig=color;
    else if(btn==ui->wk_fg) newer.wk_fg=color;
    else if(btn==ui->sb_bg) newer.sb_bg=color;
    else if(btn==ui->sb_ig) newer.sb_ig=color;
    else if(btn==ui->sb_fg) newer.sb_fg=color;
    else if(btn==ui->lb_bg) newer.lb_bg=color;
    else if(btn==ui->lb_ig) newer.lb_ig=color;
    else if(btn==ui->lb_fg) newer.lb_fg=color;

    this->updateAppearance();
}

void Setting::selectFont(QFont font){
    newer.font = ui->font_fm->currentFont();
}

void Setting::save(){
    general = {
        this->ui->autoStartTimer->isChecked(),
        this->ui->miniAfterStart->isChecked(),
        this->ui->alwaysOnTheTop->isChecked(),
        this->ui->restartRoutine->isChecked(),
        this->ui->popUpEachEpoch->isChecked(),
        this->ui->showStartPanel->isChecked(),
    };

    newer.font.setPointSize(this->ui->font_sz->value());

    older = newer;

    routine = {ui->focus->value()*60,
               ui->short_break->value()*60,
               ui->long_break->value()*60,
               ui->round->value()};

    QSettings settings("myapp.ini", QSettings::IniFormat);

    settings.beginGroup("General");
    settings.setValue("autoStartTimer", general.autoStartTimer);
    settings.setValue("miniAfterStart", general.miniAfterStart);
    settings.setValue("alwaysOnTheTop", general.alwaysOnTheTop);
    settings.setValue("restartRoutine", general.restartRoutine);
    settings.setValue("popUpEachEpoch", general.popUpEachEpoch);
    settings.setValue("showStartPanel", general.showStartPanel);
    settings.endGroup();

    settings.beginGroup("Time");
    settings.setValue("focus", routine.work);
    settings.setValue("round", routine.round);
    settings.setValue("short_break", routine.short_break);
    settings.setValue("long_break", routine.long_break);
    settings.endGroup();

    settings.beginGroup("Appearance");
    settings.setValue("wk_bg", newer.wk_bg.name());
    settings.setValue("wk_ig", newer.wk_ig.name());
    settings.setValue("wk_fg", newer.wk_fg.name());
    settings.setValue("sb_bg", newer.sb_bg.name());
    settings.setValue("sb_ig", newer.sb_ig.name());
    settings.setValue("sb_fg", newer.sb_fg.name());
    settings.setValue("lb_bg", newer.lb_bg.name());
    settings.setValue("lb_ig", newer.lb_ig.name());
    settings.setValue("lb_fg", newer.lb_fg.name());
    settings.setValue("font_family", newer.font.family());
    settings.setValue("font_size", newer.font.pointSize());
    settings.endGroup();

    close();
}

Setting::~Setting()
{
    delete ui;
}
