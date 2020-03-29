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

private:
    Ui::CustomActuatorControl *ui;
};

#endif // CUSTOMACTUATORCONTROL_H
