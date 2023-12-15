#ifndef SETTING_H
#define SETTING_H

#include <QDialog>

namespace Ui {
class Setting;
}

struct RotSetting{
    int work;
    int short_break;
    int long_break;
    int round;
};

struct GenSetting{
    bool autoStartTimer;
    bool miniAfterStart;
    bool alwaysOnTheTop;
    bool restartRoutine;
    bool popUpEachEpoch;
    bool showStartPanel;
};

struct ApaSetting
{
    QFont font;
    QColor wk_bg, wk_ig, wk_fg, sb_bg, sb_ig, sb_fg, lb_bg, lb_ig, lb_fg;

    ApaSetting():
        wk_bg(QColor(255, 255, 255)),
        wk_ig(QColor(200, 200, 200)),
        wk_fg(QColor(0,98,132)), // 飘
        sb_bg(QColor(46, 47, 48)),
        sb_ig(QColor(177, 180, 121)),
        sb_fg(QColor(255, 255, 255)),
        lb_bg(QColor(38, 135, 133)), // 清碧
        lb_ig(QColor(165, 222, 228)), // 瓶觐
        lb_fg(QColor(129, 199, 212)) // 水
    {

    }
};

class Setting : public QDialog
{
    Q_OBJECT

    ApaSetting newer, &older;
    RotSetting &routine;
    GenSetting &general;

    void updateRoutine();
    void updateGeneral();
    void updateAppearance();

public:
    explicit Setting(RotSetting &current_routine, GenSetting &current_general, ApaSetting &current, QWidget *parent = nullptr);
    ~Setting();

private:
    Ui::Setting *ui;
private slots:
    void selectColor();
    void selectFont(QFont);

    void save();
};

enum WorkStatus { Work, SBreak, LBreak };

struct Status
{
    WorkStatus work_status;
    int focus_times, short_break_times, long_break_times, round;
};


#endif // SETTING_H
