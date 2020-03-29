#ifndef WEBCAM_H
#define WEBCAM_H

#include <QCamera>
#include <QWidget>
#include <QCameraViewfinder>
#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>
#include <QPushButton>

class WebCam : public QWidget
{
    Q_OBJECT
public:


    WebCam();
    ~WebCam();
    void setEnabledButtons(bool);

private:
    QCamera *m_camera = nullptr;
    QCameraViewfinder *viewfinder = nullptr;
    QLabel *label = nullptr, *udControlLabel, *lrControlLabel;
    QVBoxLayout *layout, *butLayout, *camLayout, *udControlLayout;
    QHBoxLayout *layout_panel, *cam2Layout, *lrContriolLayout;
    QSpacerItem *spacer;
    QSlider *sliderUpDown, *sliderLeftRight;
    QPushButton *startStopButton,*updateButton;

    struct{
        int up_down=90;
        int left_right=90;
    }controls_value;


private slots:
    void on_startStopButton_clicked();
    void on_updateButton_clicked();
    void on_sliderUpDown_valueChanged();
    void on_sliderLeftRight_valueChanged();

signals:
    void send_control(QString, int);
};

#endif // WEBCAM_H
