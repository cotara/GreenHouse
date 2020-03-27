#include "myplot.h"

#include <QWidget>
#include "axistag.h"

#include <QPushButton>
#include <QToolButton>
#include <QSpacerItem>

MyPlot::MyPlot(int type,QWidget *parent) : QWidget(parent)

{
    layout = new QVBoxLayout;
    butLayout = new QHBoxLayout;
    mPlot = new QCustomPlot(this);
    spacer = new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Fixed);

    setLayout(layout);
    layout->addWidget(mPlot);
    layout->addLayout(butLayout);
    butLayout->addSpacerItem(spacer);

    if (type == 0){
        startStopButton = new QPushButton;
        butLayout->addWidget(startStopButton);
        clearButton = new QPushButton;
        butLayout->addWidget(clearButton);
        startStopButton->setText("Строить график");
        startStopButton->setEnabled(false);
        clearButton->setText("Очистить график");
        clearButton->setEnabled(false);
        connect(startStopButton, &QPushButton::clicked, this, &MyPlot::on_StartGraphButtun_clicked);
        connect(clearButton, &QPushButton::clicked, this, &MyPlot::on_ClearGraphButton_clicked);
    }
    else if (type == 1){
        chooseFileButton = new QToolButton;
        butLayout->addWidget(chooseFileButton);
        chooseFileButton->setText("Посмотреть историю");

        connect(chooseFileButton, &QToolButton::clicked, this, &MyPlot::on_chooseFileButton_clicked);

    }

    mPlot->setMinimumWidth(300);
    // configure plot to have two right axes:
    mPlot->yAxis->setTickLabels(false);
    mPlot->xAxis->setRange(mPlot->xAxis->range().upper, 300, Qt::AlignRight);
    mPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);

    //connect(mPlot->yAxis2, SIGNAL(rangeChanged(QCPRange)), mPlot->yAxis, SLOT(setRange(QCPRange))); // left axis only mirrors inner right axis
    // connect slot that ties some axis selections together (especially opposite axes):
    connect(mPlot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
    // connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
    connect(mPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
    connect(mPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));

    // make bottom and left axes transfer their ranges to top and right axes:
    connect(mPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), mPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(mPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), mPlot->yAxis2, SLOT(setRange(QCPRange)));

    mPlot->yAxis2->setVisible(true);
    mPlot->axisRect()->addAxis(QCPAxis::atRight);
    mPlot->axisRect()->addAxis(QCPAxis::atRight);
    mPlot->axisRect()->axis(QCPAxis::atRight, 0)->setPadding(20); // add some padding to have space for tags
    mPlot->axisRect()->axis(QCPAxis::atRight, 1)->setPadding(20); // add some padding to have space for tags
    mPlot->axisRect()->axis(QCPAxis::atRight, 2)->setPadding(20);

    // create graphs:
    mGraph1 = mPlot->addGraph(mPlot->xAxis, mPlot->axisRect()->axis(QCPAxis::atRight, 0));
    mGraph2 = mPlot->addGraph(mPlot->xAxis, mPlot->axisRect()->axis(QCPAxis::atRight, 1));
    mGraph3 = mPlot->addGraph(mPlot->xAxis, mPlot->axisRect()->axis(QCPAxis::atRight, 2));


    mGraph1->setPen(QPen(QColor(250, 120, 0)));
    mGraph2->setPen(QPen(QColor(0, 180, 60)));
    mGraph3->setPen(QPen(QColor(0, 0, 60)));

    // create tags with newly introduced AxisTag class (see axistag.h/.cpp):
    mTag1 = new AxisTag(mGraph1->valueAxis());
    mTag1->setPen(mGraph1->pen());
    mTag2 = new AxisTag(mGraph2->valueAxis());
    mTag2->setPen(mGraph2->pen());
    mTag3 = new AxisTag(mGraph3->valueAxis());
    mTag3->setPen(mGraph3->pen());
}

MyPlot::~MyPlot()
{
//    delete layout;
//    delete butLayout;
//    delete mPlot;
//    delete startStopButton;
//    delete clearButton;
//    delete spacer;
//    delete mTag1;
//    delete mTag2;
//    delete mTag3;
}

void MyPlot::selectionChanged()
{
  /*
   normally, axis base line, axis tick labels and axis labels are selectable separately, but we want
   the user only to be able to select the axis as a whole, so we tie the selected states of the tick labels
   and the axis base line together. However, the axis label shall be selectable individually.

   The selection state of the left and right axes shall be synchronized as well as the state of the
   bottom and top axes.

   Further, we want to synchronize the selection of the graphs with the selection state of the respective
   legend item belonging to that graph. So the user can select a graph by either clicking on the graph itself
   or on its legend item.
  */

  // make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (mPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || mPlot->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      mPlot->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || mPlot->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    mPlot->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    mPlot->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (mPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || mPlot->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      mPlot->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || mPlot->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    mPlot->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    mPlot->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }

  // synchronize selection of graphs with selection of corresponding legend items:
  for (int i=0; i<mPlot->graphCount(); ++i)
  {
    QCPGraph *graph = mPlot->graph(i);
    QCPPlottableLegendItem *item = mPlot->legend->itemWithPlottable(graph);
    if (item->selected() || graph->selected())
    {
      item->setSelected(true);
      graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
    }
  }
}
void MyPlot::mousePress()
{
  // if an axis is selected, only allow the direction of that axis to be dragged
  // if no axis is selected, both directions may be dragged

  if (mPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    mPlot->axisRect()->setRangeDrag(mPlot->xAxis->orientation());
  else if (mPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    mPlot->axisRect()->setRangeDrag(mPlot->yAxis->orientation());
  else
    mPlot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}
void MyPlot::mouseWheel()
{
  // if an axis is selected, only allow the direction of that axis to be zoomed
  // if no axis is selected, both directions may be zoomed

  if (mPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    mPlot->axisRect()->setRangeZoom(mPlot->xAxis->orientation());
  else if (mPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    mPlot->axisRect()->setRangeZoom(mPlot->yAxis->orientation());
  else
    mPlot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}
void MyPlot::paint_graphs(const Data &data)
{
    double rangeX = mPlot->xAxis->range().size();

    if (plot){
        // calculate and add a new data point to each graph:
        mGraph1->addData(mGraph1->dataCount(), data.a);
        mGraph2->addData(mGraph2->dataCount(), data.b);
        mGraph3->addData(mGraph3->dataCount(), data.c);


        // make key axis range scroll with the data:
        mPlot->xAxis->rescale();
        //mPlot->rescaleAxes();
        mGraph1->rescaleValueAxis(false, true);
        mGraph2->rescaleValueAxis(false, true);
        mGraph3->rescaleValueAxis(false, true);

        mPlot->xAxis->setRange(mPlot->xAxis->range().upper, rangeX, Qt::AlignRight);
        // update the vertical axis tag positions and texts to match the rightmost data point of the graphs:
        double graph1Value = mGraph1->dataMainValue(mGraph1->dataCount()-1);
        double graph2Value = mGraph2->dataMainValue(mGraph2->dataCount()-1);
        double graph3Value = mGraph3->dataMainValue(mGraph3->dataCount()-1);
        mTag1->updatePosition(graph1Value);
        mTag2->updatePosition(graph2Value);
        mTag3->updatePosition(graph3Value);
        mTag1->setText(QString::number(graph1Value, 'f', 2));
        mTag2->setText(QString::number(graph2Value, 'f', 2));
        mTag3->setText(QString::number(graph3Value, 'f', 2));

        mPlot->yAxis2->setRange(15, 35);
        //mPlot->yAxis2->setRange(0, 100);
        mPlot->axisRect()->axis(QCPAxis::atRight, 1)->setRange(65,85);
        //mPlot->axisRect()->axis(QCPAxis::atRight, 1)->setRange(0,100);
        mPlot->axisRect()->axis(QCPAxis::atRight, 2)->setRange(-400,2000);


        mPlot->replot();
    }
}

void MyPlot::paint_histiory(QVector<double> a, QVector<double> b, QVector<double> c)
{
    QVector<double> x;

    for (int i =0;i<a.size();i++){
        x.append(i);
    }
// calculate and add a new data point to each graph:
//        mGraph1->addData(mGraph1->dataCount(), data.a);
//        mGraph2->addData(mGraph2->dataCount(), data.b);
//        mGraph3->addData(mGraph3->dataCount(), data.c);

        mPlot->graph(0)->setData(x, a);
        mPlot->graph(1)->setData(x, b);
        mPlot->graph(2)->setData(x, c);

//        // make key axis range scroll with the data:
//        mPlot->xAxis->rescale();
//        //mPlot->rescaleAxes();
//        mGraph1->rescaleValueAxis(false, true);
//        mGraph2->rescaleValueAxis(false, true);
//        mGraph3->rescaleValueAxis(false, true);

        mPlot->rescaleAxes();

        //mPlot->xAxis->setRange(mPlot->xAxis->range().upper, rangeX, Qt::AlignRight);

        // update the vertical axis tag positions and texts to match the rightmost data point of the graphs:
//        double graph1Value = mGraph1->dataMainValue(mGraph1->dataCount()-1);
//        double graph2Value = mGraph2->dataMainValue(mGraph2->dataCount()-1);
//        double graph3Value = mGraph3->dataMainValue(mGraph3->dataCount()-1);
//        mTag1->updatePosition(graph1Value);
//        mTag2->updatePosition(graph2Value);
//        mTag3->updatePosition(graph3Value);
//        mTag1->setText(QString::number(graph1Value, 'f', 2));
//        mTag2->setText(QString::number(graph2Value, 'f', 2));
//        mTag3->setText(QString::number(graph3Value, 'f', 2));

//        mPlot->yAxis2->setRange(15, 35);
//        //mPlot->yAxis2->setRange(0, 100);
//        mPlot->axisRect()->axis(QCPAxis::atRight, 1)->setRange(65,85);
//        //mPlot->axisRect()->axis(QCPAxis::atRight, 1)->setRange(0,100);
//        mPlot->axisRect()->axis(QCPAxis::atRight, 2)->setRange(-400,2000);


        mPlot->replot();
}
void MyPlot::activateButtons()
{
    if (startStopButton!=nullptr){
        startStopButton->setEnabled(true);
        startStopButton->setText("Остановить график");
        plot=true;
    }
    if (clearButton!=nullptr)
        clearButton->setEnabled(true);

}

void MyPlot::diactivateButtons()
{
    if (startStopButton!=nullptr)
        startStopButton->setEnabled(false);
    if (clearButton!=nullptr)
        clearButton->setEnabled(false);
}

void MyPlot::on_StartGraphButtun_clicked()
{
    if (plot){
        plot=false;
        startStopButton->setText("Строить график");
    }
    else{
        plot=true;
        startStopButton->setText("Остановить график");
    }
}

void MyPlot::on_ClearGraphButton_clicked()
{
    for (int i=0;i<3;i++)
        mPlot->graph(i)->data().data()->clear();

    mPlot->replot();
}

void MyPlot::on_chooseFileButton_clicked()
{
    if (file == nullptr)
        file = new QFile();

    filename = QFileDialog::getOpenFileName(this, "Посмотреть данные ");

    if (filename.isEmpty()){
        QMessageBox::critical(nullptr,"Ошибка!","Укажите директорию!");
        return;
    }
    file->setFileName(filename);
    if(file->open(QIODevice::ReadOnly) == true){
        QVector<double> a,b,c,temp;

        QTextStream in(file);
        while( !in.atEnd())
        {
            temp=lineParser(in.readLine());
            a.append(temp.at(0));
            b.append(temp.at(1));
            c.append(temp.at(2));
            line_count++;
        }
        paint_histiory(a,b,c);
    }

    else
        return;


    //paint_histiory();
    //ui->SaveLogsLabel->setText(log->get_dirname());
}

QVector<double> MyPlot::lineParser(QString s)
{
    QVector<double> abc;

    QList<QByteArray> list;
    QByteArray b=s.toUtf8();

    list=b.split(' ');

    for (QByteArray i : list){
        if (i.length() > 0){
            if (i.at(0) == 'T'){
                QList<QByteArray> temp = i.split('=');
                if (temp.length() > 1)
                    abc.append(temp.at(1).toDouble());
            }
            else if(i.at(0) == 'V'){
                QList<QByteArray> temp = i.split('=');
                if (temp.length() > 1)
                    abc.append(temp.at(1).toDouble());
            }
            else if(i.at(0) == 'L'){
                QList<QByteArray> temp = i.split('=');
                if (temp.length() > 1)
                    abc.append(temp.at(1).toDouble());
            }
        }
    }
    return abc;
}
