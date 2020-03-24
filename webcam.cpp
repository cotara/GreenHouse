#include "webcam.h"
#include <QCamera>
#include <QCameraViewfinder>



WebCam::WebCam()
{
    viewfinder = new QCameraViewfinder;
    layout = new QVBoxLayout;
    startStopButton = new QPushButton;
    updateButton = new QPushButton;
    butLayout = new QHBoxLayout;
    spacer = new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Fixed);

    setLayout(layout);
    layout->addLayout(butLayout);
    butLayout->addSpacerItem(spacer);
    butLayout->addWidget(startStopButton);
    startStopButton->setText("Запустить трансляцию");
    startStopButton->setEnabled(false);
    butLayout->addWidget(updateButton);
    updateButton->setText("Обновить камеру");


    connect(startStopButton, &QPushButton::clicked, this, &WebCam::on_startStopButton_clicked);
    connect(updateButton, &QPushButton::clicked, this, &WebCam::on_updateButton_clicked);

        QList<QByteArray> cameras = QCamera::availableDevices();
        if (!cameras.isEmpty()){
            m_camera = new QCamera( cameras.first());
            layout->insertWidget(0,viewfinder);
            viewfinder->setMinimumSize( 50, 50 );
            m_camera->setViewfinder( viewfinder );
            m_camera->setCaptureMode( QCamera::CaptureStillImage );
            m_camera->start();        
            startStopButton->setText("Остановить трансляцию");
            startStopButton->setEnabled(true);
        }
        else{
            label = new QLabel;
            layout->insertWidget(0,label);
            label->setText("Веб-камера недоступна");
        }
}
WebCam::~WebCam(){
    delete label;
    delete viewfinder;
    delete layout;
    delete startStopButton;
    delete updateButton;
    delete butLayout;
    delete spacer;
}

void WebCam::on_startStopButton_clicked()
{
    if (m_camera->status() == QCamera::ActiveStatus ){
        startStopButton->setText("Запустить трансляцию");
        if(m_camera!=nullptr)
            m_camera->stop();
    }
    else if (m_camera->status() == QCamera::LoadedStatus){
        startStopButton->setText("Остановить трансляцию");
        if(m_camera!=nullptr)
            m_camera->start();
    }
}

void WebCam::on_updateButton_clicked()
{
    if (viewfinder != nullptr){
        delete viewfinder;
        viewfinder = nullptr;
    }
    if(m_camera != nullptr){
        if (m_camera->status() == QCamera::ActiveStatus )
            m_camera->stop();
        //m_camera->deleteLater();
        delete m_camera;
        m_camera = nullptr;
    }

    QList<QByteArray> cameras = QCamera::availableDevices();
    if (!cameras.isEmpty()){
        m_camera = new QCamera( cameras.first());
        viewfinder = new QCameraViewfinder;
        layout->insertWidget(0,viewfinder);
        viewfinder->setMinimumSize( 50, 50 );
        m_camera->setViewfinder( viewfinder );
        m_camera->setCaptureMode( QCamera::CaptureStillImage );
        m_camera->start();
        startStopButton->setText("Остановить трансляцию");
        startStopButton->setEnabled(true);
        if (label!=nullptr)
            delete label;
        label = nullptr;
    }
    else{
        if (label!=nullptr){
            delete label;
            label = nullptr;
        }
        label = new QLabel;
        layout->insertWidget(0,label);
        label->setText("Веб-камера недоступна");
        startStopButton->setEnabled(false);
    }
}

