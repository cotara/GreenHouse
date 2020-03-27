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
    QLabel *label = nullptr, *sliderLabel;
    QVBoxLayout *layout, *butLayout, *layout_left,*layout_center,*layout_right;
    QHBoxLayout *layout_control,*layout_panel;
    QSpacerItem *spacer;
    QSlider *slider;
    QPushButton *startStopButton,*updateButton,*upButton,*downButton,*leftButton,*rightButton;

    struct{
        int up_down=90;
        int left_right=90;
        int k=5;
    }controls_value;


private slots:
    void on_startStopButton_clicked();
    void on_updateButton_clicked();
    void on_slider_valueChanged();

public slots:
    void on_downButton_clicked();
    void on_upButton_clicked();
    void on_leftButton_clicked();
    void on_rightButton_clicked();

signals:
    void send_control(QString, int);
};

#endif // WEBCAM_H
