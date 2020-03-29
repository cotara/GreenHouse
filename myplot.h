#ifndef MYPLOT_H
#define MYPLOT_H

#include "qcustomplot.h"
#include "axistag.h"
#include <QVBoxLayout>
#include <QVector>

class MyPlot : public QWidget
{
    Q_OBJECT
public:
    //explicit MyPlot(QWidget *parent = nullptr);
    explicit MyPlot(int type = 0, QWidget *parent = nullptr);
    ~MyPlot();
    struct Data{

        Data(double x=0, double y=0, double z=0):a(x), b(y), c(z){}

        double a;
        double b;
        double c;
    };
    void paint_graphs(const Data &data);
    void paint_histiory(QVector<double> a,QVector<double> b, QVector<double> c);
    void activateButtons();
    void diactivateButtons();

signals:

public slots:

private slots:
    void mousePress();
    void mouseWheel();
    void selectionChanged();
    void on_StartGraphButtun_clicked();
    void on_ClearGraphButton_clicked();
    void on_chooseFileButton_clicked();
private:
    QCustomPlot *mPlot;
    QVBoxLayout *layout;
    QHBoxLayout *butLayout;
    QSpacerItem *spacer;
    QPushButton *startStopButton,*clearButton;
    QToolButton *chooseFileButton;
    QPointer<QCPGraph> mGraph1;
    QPointer<QCPGraph> mGraph2;
    QPointer<QCPGraph> mGraph3;
    AxisTag *mTag1;
    AxisTag *mTag2;
    AxisTag *mTag3;
    bool plot=true;
    int widget_type;
    QFile *file  = nullptr;
    QString filename;
    int line_count=0;

    QVector<double> lineParser(QString s);

};

#endif // MYPLOT_H
