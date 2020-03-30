#ifndef CUSTOMACTUATORCONTROL_H
#define CUSTOMACTUATORCONTROL_H

#include <QWidget>

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
    void on_pushButton_clicked();
    void on_HeaterOncheckBox_stateChanged(int arg1);
    void temperatureChanged(double);

    void on_checkBox_2_stateChanged(int arg1);

    void on_fanSpeedCheckBox_stateChanged(int arg1);

    void on_fanSpeedSlider_sliderReleased();

private:
    Ui::CustomActuatorControl *ui;
    QMovie *moHeater,*moCooler;

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
    void send_control(QString,int);
    void send_control(QString,int,int,int);
};

#endif // CUSTOMACTUATORCONTROL_H
