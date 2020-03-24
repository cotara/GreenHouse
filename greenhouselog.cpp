#include "greenhouselog.h"
#include <QTimer>
#include <QMessageBox>

GreenHouseLog::GreenHouseLog()
{
    timer.setInterval(43200000);
    timer.start();
    connect(&timer,&QTimer::timeout,this,&GreenHouseLog::realTimeout);
    update_filename();//
}

GreenHouseLog::~GreenHouseLog(){}

bool GreenHouseLog::create_file()
{
    if (dirname.isEmpty()){
        return false;
        QMessageBox::critical(nullptr,"Ошибка!","Укажите директорию!");
    }
    update_filename();
    file.setFileName(dirname + "/" + filename);
    if(file.open(QIODevice::WriteOnly) == true)
        return true;
    else
        return false;
}

void GreenHouseLog::update_filename(){
    set_filename(filename = QDate::currentDate().toString("yyyy_MM_dd") +
            QTime::currentTime().toString("__hh_mm_ss")+
                     ".txt");
}
bool GreenHouseLog::start_logging()
{
    if (create_file()){
        logging = true;
        return true;
    }
    else{
        QMessageBox::critical(nullptr,"Ошибка!","Немозможно открыть файл: " + filename);
        logging = false;
        return false;
    }
}

bool GreenHouseLog::stop_logging(){
    file.close();
    logging = false;
    return true;
}

void GreenHouseLog::set_logging_state(bool state)
{
    logging = state;
}

bool GreenHouseLog::get_logging_state()
{
    return logging;
}

bool GreenHouseLog::wrire_to_file(QByteArray &str)
{
    if (file.isOpen()){
        file.write(str);
        file.flush();
        return true;
    }
    else{
        QMessageBox::critical(nullptr,"Ошибка!","Файл " + filename + "не открыт");
        return  false;
    }
}
void GreenHouseLog::realTimeout(){
                stop_logging();
                update_filename();
                start_logging();
}
void GreenHouseLog::set_filename(QString name)
{
    filename=name;
}
void GreenHouseLog::set_dirname(QString dir)
{
    dirname=dir;
}
QString GreenHouseLog::get_filename()
{
    return filename;
}
QString GreenHouseLog::get_dirname()
{
    return dirname;
}


