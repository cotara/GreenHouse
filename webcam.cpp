#include "webcam.h"
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraInfo>


WebCam::WebCam()
{
    viewfinder = new QCameraViewfinder;
    layout = new QVBoxLayout;
    layout_panel = new QHBoxLayout;
    startStopButton = new QPushButton;
    updateButton = new QPushButton;
    leftButton = new QPushButton;
    rightButton = new QPushButton;
    upButton = new QPushButton;
    downButton = new QPushButton;
    butLayout = new QVBoxLayout;
    layout_control = new QHBoxLayout;
    layout_left = new QVBoxLayout;
    layout_center = new QVBoxLayout;
    layout_right = new QVBoxLayout;
    spacer = new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Fixed);

    setLayout(layout);
    layout->addLayout(layout_panel);
    layout_panel->addSpacerItem(spacer);

    layout_panel->addLayout(layout_control);
        layout_control->addLayout(layout_left);
            layout_left->addWidget(leftButton);
        layout_control->addLayout(layout_center);
            layout_center->addWidget(upButton);
            layout_center->addWidget(downButton);
        layout_control->addLayout(layout_right);
            layout_right->addWidget(rightButton);

    layout_panel->addLayout(butLayout);
        butLayout->addWidget(startStopButton);
        startStopButton->setText("Запустить трансляцию");
        startStopButton->setEnabled(false);
        butLayout->addWidget(updateButton);
        updateButton->setText("Обновить камеру");

    leftButton->setIcon(QPixmap( ":/new/Resources/left.png"));
    rightButton->setIcon(QPixmap( ":/new/Resources/right.png"));
    upButton->setIcon(QPixmap( ":/new/Resources/up.png"));
    downButton->setIcon(QPixmap( ":/new/Resources/down.png"));
    leftButton->setEnabled(false);
    rightButton->setEnabled(false);
    upButton->setEnabled(false);
    downButton->setEnabled(false);
    connect(leftButton, &QPushButton::clicked, this, &WebCam::on_leftButton_clicked);
    connect(rightButton, &QPushButton::clicked, this, &WebCam::on_rightButton_clicked);
    connect(upButton, &QPushButton::clicked, this, &WebCam::on_upButton_clicked);
    connect(downButton, &QPushButton::clicked, this, &WebCam::on_downButton_clicked);

    connect(startStopButton, &QPushButton::clicked, this, &WebCam::on_startStopButton_clicked);
    connect(updateButton, &QPushButton::clicked, this, &WebCam::on_updateButton_clicked);

    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
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
//    delete label;
//    delete viewfinder;
//    delete layout;
//    delete startStopButton;
//    delete updateButton;
//    delete butLayout;
    //    delete spacer;
}

void WebCam::setEnabledButtons(bool val)
{
    leftButton->setEnabled(val);
    rightButton->setEnabled(val);
    upButton->setEnabled(val);
    downButton->setEnabled(val);
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

    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    if (!cameras.isEmpty()){
        m_camera = new QCamera(cameras.first());
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

void WebCam::on_downButton_clicked()
{
    controls_value.up_down--;
    emit send_control("UD",controls_value.up_down);
}

void WebCam::on_upButton_clicked()
{
    controls_value.up_down++;
    emit send_control("UD",controls_value.up_down);
}

void WebCam::on_leftButton_clicked()
{
    controls_value.left_right--;
    emit send_control("LR",controls_value.left_right);
}

void WebCam::on_rightButton_clicked()
{
    controls_value.left_right++;
    emit send_control("LR",controls_value.left_right);
}

