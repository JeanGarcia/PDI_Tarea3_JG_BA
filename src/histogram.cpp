#include "histogram.h"
#include "ui_histogram.h"

Histogram::Histogram(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Histogram)
{
    ui->setupUi(this);
}

Histogram::~Histogram()
{
    delete ui;
}

void Histogram::LoadHist(int r[], int g[], int b[], QImage Image){
    int max = r[0];
    QVector<double> x(256), y(256);

//RED
    for (int i=0; i<256; ++i){
      x[i] = i;
      y[i] = r[i];
      if(r[i] > max)
          max = r[i];
    }
    // create graph and assign data to it:
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setPen(QPen(Qt::red));
    ui->customPlot->graph(0)->setData(x, y);
//GREEN
    for (int i=0; i<256; ++i){
        y[i] = g[i];
        if(g[i] > max)
            max = g[i];
    }
    ui->customPlot->addGraph();
    ui->customPlot->graph(1)->setPen(QPen(Qt::green));
    ui->customPlot->graph(1)->setData(x, y);
//BLUE
    for (int i=0; i<256; ++i){
        y[i] = b[i];
        if(b[i] > max)
            max = b[i];
    }
    ui->customPlot->addGraph();
    ui->customPlot->graph(2)->setPen(QPen(Qt::blue));
    ui->customPlot->graph(2)->setData(x, y);
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(0, 255);
    ui->customPlot->yAxis->setRange(0, max);
    ui->customPlot->replot();
}
