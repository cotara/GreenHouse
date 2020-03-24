 #ifndef GREENHOUSE_H
#define GREENHOUSE_H

#include <QMainWindow>
#include "serialsettings.h"
#include "console.h"
#include <QTime>
#include <QLabel>
#include <QTimer>
#include <QFile>
#include "greenhouselog.h"
#include <QVector>
#include <QtQuickWidgets/QQuickWidget>
#include "myplot.h"
#include "webcam.h"

namespace Ui {
class GreenHouse;
}

class GreenHouse : public QMainWindow
{
    Q_OBJECT

public:
    explicit GreenHouse(QWidget *parent = nullptr);
    ~GreenHouse();

private slots:
    void on_Settings_triggered();
    void on_Connect_triggered();
    void on_Disconnect_triggered();
    void realTimeout();
    void on_ChooseDirDialog_clicked();
    void on_pushButton_clicked();//Старт запись

private:
    struct state{
        QTime time;
        MyPlot::Data myPlotData;
        double &temperature = myPlotData.a;
        double &humidity = myPlotData.b;
        double &illumination = myPlotData.c;
        bool bulb;
        bool windows;
        bool pump;
        bool heat;


    }now_state;

    Ui::GreenHouse *ui;
    SerialSettings *settings_ptr;
    QSerialPort *serial;
    GreenHouseLog *log = nullptr;    
    QQuickWidget *temperatureQW;
    QQuickWidget *humidityQW;
    QQuickWidget *illuminationQW;
    WebCam *mCam;
    MyPlot *mPlot, *historyPlot;


    void readData();
    void parser(const QByteArray &str);
    void set_temperature_dial();
    void set_humidity_dial();
    void set_illumination_dial();
    void init_qml_wingets();


    QLabel RealTime,Status;
    QTimer timer;

};
#endif // GREENHOUSE_H