#include "customactuatorcontrol.h"
#include "ui_customactuatorcontrol.h"
#include <QColor>
#include <QColorDialog>
#include <QStyleFactory>
#include <QMovie>
#include <QDebug>

CustomActuatorControl::CustomActuatorControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomActuatorControl)
{
    ui->setupUi(this);

    ui->pushButton->setStyle(QStyleFactory::create("windows"));//штоб кнопка цвет меняла
    moHeater = new QMovie();
    moCooler = new QMovie();
    connect(this,&CustomActuatorControl::temperatureChange,&CustomActuatorControl::temperatureChanged);

    ui->pushButton->setEnabled(false);
    ui->fanSpeedSlider->setEnabled(false);
    ui->fanSpeedLabel->setNum(controls_value.cooler);
}

CustomActuatorControl::~CustomActuatorControl()
{
    delete ui;
}

void CustomActuatorControl::on_pushButton_clicked(){

    QColor color = QColorDialog::getColor(Qt::white);
    if (!color.isValid() ) {
    // Cancel
    }
    else{
        int r,b,g;
        color.getRgb(&r,&g,&b);
        QPalette myPalette = ui->pushButton->palette();
        myPalette.setColor( QPalette::Button, color);

        ui->Red->setNum(r);
        ui->Green->setNum(g);
        ui->Blue->setNum(b);
        if (r+g+b < 127*3){
            myPalette.setColor( QPalette::ButtonText,Qt::white);
        }
        else
            myPalette.setColor( QPalette::ButtonText,Qt::black);

        ui->pushButton->setPalette( myPalette );
    }

}

void CustomActuatorControl::on_HeaterOncheckBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked){
        moHeater->setFileName(":/new/Resources/Heater.gif");
        ui->HeaterMovielabel->setMovie(moHeater);
        moHeater->start();

    }
    else{
        moHeater->stop();
        ui->HeaterMovielabel->setPixmap(QPixmap(":/new/Resources/noHeater.png"));
    }
}

void CustomActuatorControl::temperatureChanged(double t)
{
    ui->NowTemperature2->setNum(t);
}

void CustomActuatorControl::on_checkBox_2_stateChanged(int arg1)
{
    QPalette myPalette;
    QColor color(255,255,255);
   if (arg1 == Qt::Checked){
       myPalette.setColor( QPalette::ButtonText,Qt::black);
       ui->Red->setNum(255);
       ui->Green->setNum(255);
       ui->Blue->setNum(255);
       ui->pushButton->setEnabled(true);
   }
   else{
       color.setRgb(0,0,0);
       myPalette.setColor( QPalette::ButtonText,Qt::white);
       ui->Red->setNum(0);
       ui->Green->setNum(0);
       ui->Blue->setNum(0);
       ui->pushButton->setEnabled(false);
   }
   myPalette.setColor( QPalette::Button, color);
   ui->pushButton->setPalette( myPalette );
}

void CustomActuatorControl::on_fanSpeedCheckBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked){
        moCooler->setFileName(":/new/Resources/cooler.gif");
        ui->CoolerMovielabel->setMovie(moCooler);
        moCooler->start();
        ui->fanSpeedSlider->setEnabled(true);
        moCooler->setSpeed(controls_value.cooler);

    }
    else{
        moCooler->stop();
        ui->fanSpeedSlider->setEnabled(false);
        controls_value.cooler=0;
        ui->fanSpeedSlider->setValue(controls_value.cooler);
        ui->fanSpeedLabel->setNum(controls_value.cooler);
        emit send_control("WC",controls_value.cooler);
        qDebug() << "WC Value Changed to " << controls_value.cooler;
        ui->CoolerMovielabel->setPixmap(QPixmap(":/new/Resources/noCooler.png"));
    }
}

void CustomActuatorControl::on_fanSpeedSlider_sliderReleased()
{
    if(controls_value.cooler != ui->fanSpeedSlider->value()){
        controls_value.cooler = ui->fanSpeedSlider->value();
        ui->fanSpeedLabel->setNum(controls_value.cooler);
        moCooler->setSpeed(controls_value.cooler*6);
        emit send_control("WC",controls_value.cooler);
        qDebug() << "WC Value Changed to " << controls_value.cooler;
    }
}
