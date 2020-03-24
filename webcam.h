#ifndef WEBCAM_H
#define WEBCAM_H
#include <QCamera>
#include <QWidget>
#include <QCameraViewfinder>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

class WebCam : public QWidget
{
    Q_OBJECT
public:


    WebCam();
    ~WebCam();
private:
    QCamera *m_camera = nullptr;
    QCameraViewfinder *viewfinder = nullptr;
    QLabel *label = nullptr;
    QVBoxLayout *layout;
    QHBoxLayout *butLayout;
    QSpacerItem *spacer;
    QPushButton *startStopButton,*updateButton;

private slots:
    void on_startStopButton_clicked();
    void on_updateButton_clicked();
};

#endif // WEBCAM_H
