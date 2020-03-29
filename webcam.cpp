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
    butLayout = new QVBoxLayout;

    spacer = new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Fixed);

    setLayout(layout);
    layout->addLayout(layout_panel);

    //1
    layout_panel->addSpacerItem(spacer);

    //2
    layout_panel->addLayout(butLayout);
        butLayout->addWidget(startStopButton);
        startStopButton->setText("Запустить трансляцию");
        startStopButton->setEnabled(false);
        butLayout->addWidget(updateButton);
        updateButton->setText("Обновить камеру");

    connect(startStopButton, &QPushButton::clicked, this, &WebCam::on_startStopButton_clicked);
    connect(updateButton, &QPushButton::clicked, this, &WebCam::on_updateButton_clicked);

    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    if (!cameras.isEmpty()){
        m_camera = new QCamera( cameras.first());

        camLayout = new QVBoxLayout;
        udControlLayout = new QVBoxLayout;
        cam2Layout = new QHBoxLayout;
        lrContriolLayout = new QHBoxLayout;
        sliderUpDown = new QSlider;
        sliderLeftRight = new QSlider(Qt::Horizontal);
        udControlLabel = new QLabel;
        lrControlLabel = new QLabel;

        layout->insertLayout(0,camLayout);
            camLayout->addLayout(cam2Layout);
                cam2Layout->addWidget(viewfinder);
                cam2Layout->addLayout(udControlLayout);
                    udControlLayout->addWidget(udControlLabel);
                    udControlLayout->addWidget(sliderUpDown);
                    sliderUpDown->setValue(controls_value.up_down);
                    udControlLabel->setNum(controls_value.up_down);
                    sliderUpDown->setRange(0,180);
            camLayout->addLayout(lrContriolLayout);
                lrContriolLayout->addWidget(lrControlLabel);
                lrContriolLayout->addWidget(sliderLeftRight);
                sliderLeftRight->setValue(controls_value.left_right);
                lrControlLabel->setNum(controls_value.left_right);
                sliderLeftRight->setRange(0,180);

        connect(sliderUpDown, &QSlider::sliderReleased, this, &WebCam::on_sliderUpDown_valueChanged);
        connect(sliderLeftRight, &QSlider::sliderReleased, this, &WebCam::on_sliderLeftRight_valueChanged);


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
        delete m_camera;
        m_camera = nullptr;
    }

    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    if (!cameras.isEmpty()){
        m_camera = new QCamera(cameras.first());
        viewfinder = new QCameraViewfinder;
        cam2Layout->insertWidget(0,viewfinder);

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

void WebCam::on_sliderUpDown_valueChanged(){
    if(controls_value.up_down != sliderUpDown->value()){
        controls_value.up_down = sliderUpDown->value();
        udControlLabel->setNum(controls_value.up_down);
        emit send_control("UD",controls_value.up_down);
        qDebug() << "UD Value Changed to " << controls_value.up_down;
    }
}

void WebCam::on_sliderLeftRight_valueChanged(){
    if(controls_value.left_right != sliderLeftRight->value()){
        controls_value.left_right = sliderLeftRight->value();
        lrControlLabel->setNum(controls_value.left_right);
        emit send_control("LR",controls_value.left_right);
        qDebug() << "LR Value Changed to " << controls_value.left_right;
    }
}
