#include "customactuatorcontrol.h"
#include "ui_customactuatorcontrol.h"
#include <QColor>
#include <QColorDialog>
#include <QStyleFactory>
#include <QMovie>
#include <QDebug>
#include <QPainter>
#include <QQuickItem>
#include <QQmlContext>
CustomActuatorControl::CustomActuatorControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomActuatorControl)
{
    ui->setupUi(this);

    moHeater = new QMovie();
    moCooler = new QMovie();
    connect(this,&CustomActuatorControl::temperatureChange,&CustomActuatorControl::temperatureChanged);

    ui->ColorControl->setEnabled(false);
    ui->WindowControl->setEnabled(false);
    ui->HeatControl->setEnabled(false);
    ui->PumpControl->setEnabled(false);

    //ui->pushButton->setIcon(QPixmap(":/new/Resources/LightControl_1.png"));
    //ui->pushButton->setEnabled(false);
    ui->fanSpeedSlider->setEnabled(false);
    ui->fanSpeedLabel->setNum(controls_value.cooler);

    bulbQW = new QQuickWidget(this);
    bulbQW->setEnabled(false);
    bulbQW->rootContext()->setContextProperty("custom", this);
    bulbQW->setSource(QUrl("qrc:/qml/bulb.qml"));
    bulbQW->setResizeMode(QQuickWidget::SizeRootObjectToView);
    bulbQW->rootObject()->setProperty("bulbColor", QColor(0,0,0));
    bulbQW->setMinimumWidth(250);
    bulbQW->setMinimumHeight(270);
    bulbQW->setMaximumHeight(270);
    bulbQW->setMaximumWidth(250);
    ui->horizontalLayout_2->addWidget(bulbQW);

    //Диалог с запросом пароля
    passDialog = new PassDialog(this);
    connect(passDialog,&PassDialog::passCorrectSig, this, &CustomActuatorControl::pass_correct);
}

CustomActuatorControl::~CustomActuatorControl()
{
    delete ui;
}

void CustomActuatorControl::temperatureChanged(double t)
{
    ui->NowTemperature2->setNum(t);
}

void CustomActuatorControl::on_pushButton_clicked(){

    QColor color(controls_value.r,controls_value.g,controls_value.b);
    color = QColorDialog::getColor(color);
    if (!color.isValid() ) {
    // Cancel
    }
    else{
        color.getRgb(&controls_value.r,&controls_value.g,&controls_value.b);
        bulbQW->rootObject()->setProperty("bulbColor", color);
//        QPalette myPalette = ui->pushButton->palette();
//        myPalette.setColor( QPalette::Button, color);

        ui->Red->setNum(controls_value.r);
        ui->Green->setNum(controls_value.g);
        ui->Blue->setNum(controls_value.b);
//        if (r+g+b < 127*3){
//            myPalette.setColor( QPalette::ButtonText,Qt::white);
//        }
//        else
//            myPalette.setColor( QPalette::ButtonText,Qt::black);

//        ui->pushButton->setPalette( myPalette );

        QImage img(":/new/Resources/LightControl_2.png");
            QRgb tc = qRgb(controls_value.r,controls_value.g,controls_value.b);

            for (int i = 0; i < img.width(); i++)
            {
                for (int j = 0; j < img.height(); j++)
                {
                    if (img.pixel(i,j) == qRgb(255,255,0))
                    {
                        img.setPixel(i,j, tc);
                    }
                }
            }

        QImage px(1000,1000,QImage::Format_ARGB32); // создаём картинку
        QPainter p(&px); // создаём рисовалку и нацеливаем её на картинку
        p.drawImage(0,0,img);
        p.drawImage(0,0,QImage(":/new/Resources/LightControl_1.png")); // накладываем изображение из файла на эту картинку в координаты 0,0
        p.end(); // завершаем работу рисовалки
        //ui->pushButton->setIcon(QPixmap::fromImage(px));


        emit send_control("R","G","B",controls_value.r,controls_value.g,controls_value.b);
        qDebug() << "R = " << controls_value.r << " G = " << controls_value.g<< " B = " << controls_value.b;

    }

}
void CustomActuatorControl::on_CustomHeatCheckBox_stateChanged(int arg1)
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


void CustomActuatorControl::on_CustomBulbCheckBox_stateChanged(int arg1)
{
   if (arg1 == Qt::Checked){
       ui->Red->setNum(controls_value.r);
       ui->Green->setNum(controls_value.r);
       ui->Blue->setNum(controls_value.r);
       //ui->pushButton->setEnabled(true);
       bulbQW->setEnabled(true);

   }
   else{
       ui->Red->setNum(0);
       ui->Green->setNum(0);
       ui->Blue->setNum(0);
       //ui->pushButton->setEnabled(false);
        bulbQW->rootObject()->setProperty("bulbColor", QColor(0,0,0));
       bulbQW->setEnabled(false);
   }
}

void CustomActuatorControl::on_CustomFanCheckBox_stateChanged(int arg1)
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
        //emit send_control("WC",controls_value.cooler);
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
        //emit send_control("WC",controls_value.cooler);
        qDebug() << "WC Value Changed to " << controls_value.cooler;
    }
}

void CustomActuatorControl::on_checkBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked){
        passDialog->show();

    }
    else{
        ui->ColorControl->setEnabled(false);
        ui->WindowControl->setEnabled(false);
        ui->HeatControl->setEnabled(false);
        ui->PumpControl->setEnabled(false);
        ui->CustomFanCheckBox->setCheckState(Qt::Unchecked);
        ui->CustomBulbCheckBox->setCheckState(Qt::Unchecked);
        ui->CustomHeatCheckBox->setCheckState(Qt::Unchecked);
        ui->CustomPumpCheckBox->setCheckState(Qt::Unchecked);
    }
}


void CustomActuatorControl::pass_correct(bool value)
{
        if (value){
            ui->ColorControl->setEnabled(true);
            ui->WindowControl->setEnabled(true);
            ui->HeatControl->setEnabled(true);
            ui->PumpControl->setEnabled(true);
        }
        else{
            ui->checkBox->setCheckState(Qt::Unchecked);
            ui->ColorControl->setEnabled(false);
            ui->WindowControl->setEnabled(false);
            ui->HeatControl->setEnabled(false);
            ui->PumpControl->setEnabled(false);
        }
}

void CustomActuatorControl::on_CustomPumpCheckBox_stateChanged(int arg1)
{

}
