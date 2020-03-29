#include "customactuatorcontrol.h"
#include "ui_customactuatorcontrol.h"
#include <QColor>
#include <QColorDialog>
#include <QStyleFactory>
CustomActuatorControl::CustomActuatorControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomActuatorControl)
{
    ui->setupUi(this);

    ui->pushButton->setStyle(QStyleFactory::create("windows"));//штоб кнопка цвет меняла
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
