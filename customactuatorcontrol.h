#ifndef CUSTOMACTUATORCONTROL_H
#define CUSTOMACTUATORCONTROL_H

#include <QWidget>
#include <QtQuickWidgets/QQuickWidget>
#include <passdialog.h>

namespace Ui {
class CustomActuatorControl;
}

class CustomActuatorControl : public QWidget
{
    Q_OBJECT

public:
    explicit CustomActuatorControl(QWidget *parent = nullptr);
    ~CustomActuatorControl();

private slots:

    void temperatureChanged(double);
    void on_CustomBulbCheckBox_stateChanged(int arg1);
    void on_CustomFanCheckBox_stateChanged(int arg1);
    void on_CustomHeatCheckBox_stateChanged(int arg1);
    void on_CustomPumpCheckBox_stateChanged(int arg1);
    void on_fanSpeedSlider_sliderReleased();
    void on_checkBox_stateChanged(int arg1);


public slots:
    void on_pushButton_clicked();
    void pass_correct(bool);

private:
    Ui::CustomActuatorControl *ui;
    QMovie *moHeater,*moCooler;
    QQuickWidget *bulbQW;
    PassDialog *passDialog;
    struct{
        int r=0;
        int g=0;
        int b=0;
        int heater=0;
        int cooler = 0;
        int pump = 0;
    }controls_value;


signals:
    void temperatureChange(double);
    //void send_control(QString,int);
    void send_control(QString,QString,QString,int,int,int);
};

#endif // CUSTOMACTUATORCONTROL_H
