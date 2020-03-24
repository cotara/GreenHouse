#include "greenhouse.h"
#include "ui_greenhouse.h"
#include "serialsettings.h"
#include "console.h"
#include <QDebug>
#include <qfiledialog.h>
#include <QMessageBox>
#include <QTimer>
#include "greenhouselog.h"
#include <QQmlContext>
#include <QQuickItem>

GreenHouse::GreenHouse(QWidget *parent) :  QMainWindow(parent),
    ui(new Ui::GreenHouse){

    ui->setupUi(this);
    //Настройки COM-пота
    settings_ptr = new SerialSettings();
    serial = new QSerialPort();
    connect(serial, &QSerialPort::readyRead, this, &GreenHouse::readData);

    //Виджет консоли
    ui->ConsoleWidget->setEnabled(true);
    ui->ConsoleWidget->setMaximumWidth(350);
    ui->Disconnect->setEnabled(false);

    //Виджеты стрелочных приборов
    init_qml_wingets();

    //Статус бар
    ui->statusBar->addPermanentWidget(&RealTime);
    ui->statusBar->addPermanentWidget(&Status);


    //Настройка таймера
    timer.setInterval(1000);
    timer.start();
    connect(&timer,&QTimer::timeout,this,&GreenHouse::realTimeout);

    //Виджет графика
    mPlot = new MyPlot(0);
    ui->horizontalLayout_7->addWidget(mPlot);

    //Виджет камеры
    mCam = new WebCam();
    ui->CamLayout->addWidget(mCam);

    //Виджет лукер
    historyPlot = new MyPlot(1);
    ui->GraphFileLayout->addWidget(historyPlot);

    //Виджет табов
    ui->tabWidget->setCurrentIndex(0);

}
GreenHouse::~GreenHouse(){
    delete ui;
    delete settings_ptr;
    delete serial;
    delete mPlot;
    delete mCam;
    delete log;
}

//Обработчик таймера
void GreenHouse::realTimeout()
{
    RealTime.setText("Системное время: " + QTime::currentTime().toString());
    if(QTime::currentTime().minute() == 0)
        ui->ConsoleWidget->clear();
}

//Прием данных из COM-порта
void GreenHouse::readData()
{
    const QByteArray data = serial->readAll();
    static QByteArray stringa;
    for (char i : data){
        if (i =='\n'){
            if(!stringa.isEmpty()){
                //парсер вызывайтунг
                parser(stringa);
                stringa+="\n";
                ui->ConsoleWidget->putData(stringa);
                if( log!= nullptr){
                    if(log->get_logging_state() == true)
                        log->wrire_to_file(stringa);
                }
                stringa.clear();
            }
        }
        else
            stringa.append(i);
    }
}

//Парсер COM-порта
void GreenHouse::parser(const QByteArray &str){
    QList<QByteArray> list;
    list=str.split(' ');

    for (QByteArray i : list){
        if (i.length() > 0){
            if (i.at(0) == 'T'){
                QList<QByteArray> temp = i.split('=');
                if (temp.length() > 1){
                    now_state.temperature=temp.at(1).toDouble();
                    ui->Temperature->display(now_state.temperature);
                    set_temperature_dial();
                 }
            }
            else if(i.at(0) == 'V'){
                QList<QByteArray> temp = i.split('=');
                if (temp.length() > 1){
                    now_state.humidity=temp.at(1).toDouble();
                    ui->Humidity->display(now_state.humidity);
                    set_humidity_dial();
                }
            }
            else if(i.at(0) == 'L'){
                QList<QByteArray> temp = i.split('=');
                if (temp.length() > 1){
                    now_state.illumination=temp.at(1).toDouble();
                    ui->Illumination->display(now_state.illumination);
                    set_illumination_dial();
                }
            }
            else if(i.at(0) == 'B' && i.at(1) == 'u'){
                QList<QByteArray> temp = i.split('=');
                if (temp.length() > 1){
                    now_state.bulb=temp.at(1).toInt();
                    if (now_state.bulb){
                        QPixmap myPixmap( ":/new/Resources/lightbulb.png");
                        ui->BulbLabel_2->setPixmap(myPixmap);
                    }
                    else{
                        QPixmap myPixmap( ":/new/Resources/bulb.png");
                        ui->BulbLabel_2->setPixmap(myPixmap);
                    }
                }
            }
            else if(i.at(0) == 'W' && i.at(1) == 'i'){
                QList<QByteArray> temp = i.split('=');
                if (temp.length() > 1){
                    now_state.windows=temp.at(1).toInt();
                    if (now_state.windows){
                        QPixmap myPixmap( ":/new/Resources/lightfan.png");
                        ui->WindowsLabel_2->setPixmap(myPixmap);
                    }
                    else{
                        QPixmap myPixmap( ":/new/Resources/fan.png");
                        ui->WindowsLabel_2->setPixmap(myPixmap);
                    }
                }
            }
            else if(i.at(0) == 'P' && i.at(1) == 'u'){
                QList<QByteArray> temp = i.split('=');
                if (temp.length() > 1){
                    now_state.pump=temp.at(1).toInt();
                    if (now_state.pump){
                        QPixmap myPixmap( ":/new/Resources/lightpump.png");
                        ui->PumpLabel_2->setPixmap(myPixmap);
                    }
                    else{
                        QPixmap myPixmap( ":/new/Resources/pump.png");
                        ui->PumpLabel_2->setPixmap(myPixmap);
                    }
                }
            }
            else if(i.at(0) == 'H' && i.at(1) == 'e'){
                QList<QByteArray> temp = i.split('=');
                if (temp.length() > 1){
                    now_state.heat=temp.at(1).toInt();
                    if (now_state.heat){
                         QPixmap myPixmap( ":/new/Resources/lightheating.png");
                        ui->HeatLabel_2->setPixmap(myPixmap);
                    }
                    else{
                        QPixmap myPixmap( ":/new/Resources/heating.png");
                        ui->HeatLabel_2->setPixmap(myPixmap);
                    }
                }
            }
            else if(i.count(':') == 2){

                QList<QByteArray> temp = i.split(':');
                now_state.time.setHMS(temp.at(0).toInt(),
                                      temp.at(1).toInt(),
                                      temp.at(2).toInt());
                ui->ArduinoTime->setTime(now_state.time);
            }
        }
    }
    mPlot->paint_graphs(now_state.myPlotData);

}

//QML стрелки
void GreenHouse::init_qml_wingets()
{
    temperatureQW = new QQuickWidget(this);
    temperatureQW->setSource(QUrl("qrc:/qml/dialcontrol.qml"));
    temperatureQW->setResizeMode(QQuickWidget::SizeRootObjectToView);
    temperatureQW->rootObject()->setProperty("dialType", 0);
    temperatureQW->setMinimumWidth(200);
    temperatureQW->setMinimumHeight(200);
    temperatureQW->setMaximumHeight(300);
    ui->tempDialLayout->addWidget(temperatureQW);

    humidityQW = new QQuickWidget(this);
    humidityQW->setSource(QUrl("qrc:/qml/dialcontrol.qml"));
    humidityQW->setResizeMode(QQuickWidget::SizeRootObjectToView);
    humidityQW->rootObject()->setProperty("dialType", 1);
    humidityQW->setMinimumWidth(200);
    humidityQW->setMinimumHeight(200);
    humidityQW->setMaximumHeight(300);
    ui->humDialLayout->addWidget(humidityQW);

    illuminationQW = new QQuickWidget(this);
    illuminationQW->setSource(QUrl("qrc:/qml/dialcontrol.qml"));
    illuminationQW->setResizeMode(QQuickWidget::SizeRootObjectToView);
    illuminationQW->rootObject()->setProperty("dialType", 2);
    illuminationQW->setMinimumWidth(200);
    illuminationQW->setMinimumHeight(200);
    illuminationQW->setMaximumHeight(300);
    ui->lumDialLayout->addWidget(illuminationQW);
    set_humidity_dial();
    set_temperature_dial();
    set_illumination_dial();
}
void GreenHouse::set_temperature_dial()
{
    if (now_state.temperature<14 || now_state.temperature>38){
        temperatureQW->rootObject()->setProperty("dialErr", 1);
        temperatureQW->rootObject()->setProperty("dialValue", 180);
    }
    else{
        temperatureQW->rootObject()->setProperty("dialErr", 0);
        temperatureQW->rootObject()->setProperty("dialValue", now_state.temperature);
    }

}
void GreenHouse::set_humidity_dial()
{
    if (now_state.humidity<56 || now_state.humidity>80){
        humidityQW->rootObject()->setProperty("dialErr", 1);
        humidityQW->rootObject()->setProperty("dialValue", 180);
    }
    else{
        humidityQW->rootObject()->setProperty("dialErr", 0);
        humidityQW->rootObject()->setProperty("dialValue", now_state.humidity);
    }

}
void GreenHouse::set_illumination_dial()
{
    if (now_state.illumination<-400.0 || now_state.illumination>2000.0){
        illuminationQW->rootObject()->setProperty("dialErr", 1);
        illuminationQW->rootObject()->setProperty("dialValue", 180);
    }
    else{
        illuminationQW->rootObject()->setProperty("dialErr", 0);
        illuminationQW->rootObject()->setProperty("dialValue", now_state.illumination);
    }

}

//Настройки COM-порта
void GreenHouse::on_Settings_triggered()
{
    settings_ptr->show();
}
void GreenHouse::on_Connect_triggered()
{
    //Настраиваем компорт
    serial->setPortName(settings_ptr->getName());
    serial->setBaudRate(settings_ptr->getBoudeRate());
    serial->setDataBits(settings_ptr->getDataBits());
    serial->setParity(settings_ptr->getParity());
    serial->setStopBits(settings_ptr->getStopBits());
    if (serial->open(QIODevice::ReadWrite)){
        ui->statusBar->showMessage("Подключено к " + settings_ptr->getName());
        ui->Connect->setEnabled(false);
        ui->Settings->setEnabled(false);
        ui->Disconnect->setEnabled(true);
        mPlot->activateButtons();
    }
    else{
         ui->statusBar->showMessage("Невозможно подключиться COM-порту");
    }
}
void GreenHouse::on_Disconnect_triggered()
{
    if(serial->isOpen()){
        serial->close();
        ui->Connect->setEnabled(true);
        ui->Settings->setEnabled(true);
        ui->Disconnect->setEnabled(false);
        ui->statusBar->showMessage("Отключено от " + settings_ptr->getName());
        mPlot->diactivateButtons();
    }

}

//ПУТЬ К ЛОГИРОВАНИЮ
void GreenHouse::on_ChooseDirDialog_clicked()
{
    if (log == nullptr)
        log = new GreenHouseLog();
    log->set_dirname(QFileDialog::getExistingDirectory(this, "Сохранять логи в... ") + "/");
    ui->SaveLogsLabel->setText(log->get_dirname());
}
//СТАРТ ЛОГИРОВАНИЕ
void GreenHouse::on_pushButton_clicked()
{
    if (log == nullptr)
    {
        QMessageBox::critical(this,"Ошибка","Укажите путь записи файла!");
        return;
    }
    if (log->get_logging_state() == false){                            //стартуем логгирование
        log->start_logging();
        ui->pushButton->setText("Стоп запись");
        Status.setText("Запись в файл: " + log->get_filename());
    }
    else {                                                      //останавливаем логгирование
        log->stop_logging();
        ui->pushButton->setText("Старт запись");
        Status.setText("Запись в файл: " + log->get_filename() + " окончена");
    }
}



